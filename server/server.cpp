#include <iostream>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "../common/protocol.h"
#include "../common/json_helper.h"
#include "../database/db_manager.h"

using namespace std;

// ===== GLOBAL DATABASE =====
DBManager* db = nullptr;

// ===== IN-MEMORY CACHE FOR ONLINE USERS =====
map<int, int> socket_to_userid;        // socket -> user_id
map<int, string> socket_to_username;   // socket -> username
map<string, int> username_to_socket;   // username -> socket
map<int, string> socket_to_token;      // socket -> token

// ===== MUTEXES =====
pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// ===== HELPER FUNCTIONS =====

void send_packet(int client_socket, int command, int status, const string& json_body) {
    PacketHeader header(command, status);
    header.body_length = json_body.length();
    
    send(client_socket, &header, sizeof(PacketHeader), 0);
    if (header.body_length > 0) {
        send(client_socket, json_body.c_str(), json_body.length(), 0);
    }
}

// ===== REQUEST HANDLERS =====

void handle_register(int client_socket, const map<string, string>& body) {
    string username = body.count("username") ? body.at("username") : "";
    string pass_hash = body.count("pass_hash") ? body.at("pass_hash") : "";
    
    if (username.empty() || pass_hash.empty()) {
        map<string, string> resp;
        resp["error"] = "Missing username or pass_hash";
        send_packet(client_socket, S_RESP_REGISTER, STATUS_BAD_REQUEST, 
                   JsonHelper::build(resp));
        return;
    }
    
    pthread_mutex_lock(&db_mutex);
    
    // Check if username exists
    if (db->getUserId(username) != -1) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Username already exists";
        send_packet(client_socket, S_RESP_REGISTER, STATUS_CONFLICT, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Create user
    if (!db->createUser(username, pass_hash)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Failed to create user";
        send_packet(client_socket, S_RESP_REGISTER, STATUS_SERVER_ERROR, 
                   JsonHelper::build(resp));
        return;
    }
    
    pthread_mutex_unlock(&db_mutex);
    
    map<string, string> resp;
    resp["message"] = "Register OK";
    send_packet(client_socket, S_RESP_REGISTER, STATUS_CREATED, 
               JsonHelper::build(resp));
    
    cout << "✓ User registered: " << username << endl;
}

void handle_login(int client_socket, const map<string, string>& body) {
    string username = body.count("username") ? body.at("username") : "";
    string pass_hash = body.count("pass_hash") ? body.at("pass_hash") : "";
    
    if (username.empty() || pass_hash.empty()) {
        map<string, string> resp;
        resp["error"] = "Missing username or pass_hash";
        send_packet(client_socket, S_RESP_LOGIN, STATUS_BAD_REQUEST, 
                   JsonHelper::build(resp));
        return;
    }
    
    pthread_mutex_lock(&db_mutex);
    
    // Verify credentials
    if (!db->verifyUser(username, pass_hash)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Invalid username or password";
        send_packet(client_socket, S_RESP_LOGIN, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
        return;
    }
    
    int user_id = db->getUserId(username);
    if (user_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "User not found";
        send_packet(client_socket, S_RESP_LOGIN, STATUS_NOT_FOUND, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Create session token
    string token = db->createSession(user_id);
    if (token.empty()) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Failed to create session";
        send_packet(client_socket, S_RESP_LOGIN, STATUS_SERVER_ERROR, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Set user online
    db->setUserOnline(user_id, true);
    db->updateLastLogin(user_id);
    
    // Get online friends
    vector<string> friends_online = db->getOnlineFriends(user_id);
    
    pthread_mutex_unlock(&db_mutex);
    
    // Update in-memory cache
    pthread_mutex_lock(&clients_mutex);
    socket_to_userid[client_socket] = user_id;
    socket_to_username[client_socket] = username;
    username_to_socket[username] = client_socket;
    socket_to_token[client_socket] = token;
    pthread_mutex_unlock(&clients_mutex);
    
    // Send response
    map<string, string> resp;
    resp["token"] = token;
    string json_resp = JsonHelper::build_with_array(resp, "friends_online", friends_online);
    send_packet(client_socket, S_RESP_LOGIN, STATUS_OK, json_resp);
    
    cout << "✓ User logged in: " << username << " (ID: " << user_id << ")" << endl;
    
    // Notify online friends
    pthread_mutex_lock(&db_mutex);
    vector<string> all_friends = db->getFriends(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    for (const string& friend_name : all_friends) {
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(friend_name)) {
            int friend_socket = username_to_socket[friend_name];
            pthread_mutex_unlock(&clients_mutex);
            
            map<string, string> notify;
            notify["username"] = username;
            send_packet(friend_socket, S_NOTIFY_FRIEND_ONLINE, STATUS_OK, 
                       JsonHelper::build(notify));
        } else {
            pthread_mutex_unlock(&clients_mutex);
        }
    }
}

// ===== PASSWORD CHANGE HANDLER =====

void handle_change_password(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string old_password = body.count("old_password") ? body.at("old_password") : "";
    string new_password = body.count("new_password") ? body.at("new_password") : "";
    
    if (old_password.empty() || new_password.empty()) {
        map<string, string> resp;
        resp["message"] = "Missing old or new password";
        send_packet(client_socket, S_RESP_CHANGE_PASS, STATUS_BAD_REQUEST, 
                   JsonHelper::build(resp));
        return;
    }
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["message"] = "Invalid token";
        send_packet(client_socket, S_RESP_CHANGE_PASS, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
        return;
    }
    
    bool success = db->changePassword(user_id, old_password, new_password);
    pthread_mutex_unlock(&db_mutex);
    
    map<string, string> resp;
    if (success) {
        resp["message"] = "Password changed successfully";
        send_packet(client_socket, S_RESP_CHANGE_PASS, STATUS_OK, 
                   JsonHelper::build(resp));
        cout << "✓ Password changed for user_id: " << user_id << endl;
    } else {
        resp["message"] = "Old password is incorrect";
        send_packet(client_socket, S_RESP_CHANGE_PASS, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
    }
}

// ===== FRIEND HANDLERS =====

void handle_friend_add(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string target_username = body.count("target_username") ? body.at("target_username") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int target_id = db->getUserId(target_username);
    if (target_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Không thể tự kết bạn với chính mình
    if (target_id == user_id) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Kiểm tra đã là bạn chưa
    if (db->areFriends(user_id, target_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string from_username = db->getUsername(user_id);
    
    // Gửi lời mời kết bạn
    db->sendFriendRequest(user_id, target_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Thông báo cho target nếu online
    pthread_mutex_lock(&clients_mutex);
    if (username_to_socket.count(target_username)) {
        int target_socket = username_to_socket[target_username];
        pthread_mutex_unlock(&clients_mutex);
        
        map<string, string> notify;
        notify["from_username"] = from_username;
        send_packet(target_socket, S_NOTIFY_FRIEND_REQ, STATUS_OK, 
                   JsonHelper::build(notify));
    } else {
        pthread_mutex_unlock(&clients_mutex);
    }
    
    cout << "✓ Friend request: " << from_username << " -> " << target_username << endl;
}

void handle_friend_response(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string from_username = body.count("from_username") ? body.at("from_username") : "";
    string action = body.count("action") ? body.at("action") : "";  // "accept" or "reject"
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int from_id = db->getUserId(from_username);
    if (from_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string my_username = db->getUsername(user_id);
    
    if (action == "accept") {
        db->acceptFriendRequest(from_id, user_id);
        pthread_mutex_unlock(&db_mutex);
        
        // Thông báo cho người gửi lời mời nếu online
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(from_username)) {
            int from_socket = username_to_socket[from_username];
            pthread_mutex_unlock(&clients_mutex);
            
            map<string, string> notify;
            notify["username"] = my_username;
            send_packet(from_socket, S_NOTIFY_FRIEND_ACCEPT, STATUS_OK, 
                       JsonHelper::build(notify));
        } else {
            pthread_mutex_unlock(&clients_mutex);
        }
        
        cout << "✓ Friend request accepted: " << from_username << " <-> " << my_username << endl;
    } else {
        db->rejectFriendRequest(from_id, user_id);
        pthread_mutex_unlock(&db_mutex);
        cout << "✓ Friend request rejected: " << from_username << " -> " << my_username << endl;
    }
}

void handle_friend_list(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    vector<string> friends = db->getFriends(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Build JSON response với trạng thái online
    string json_resp = "{\"friends\":[";
    for (size_t i = 0; i < friends.size(); i++) {
        if (i > 0) json_resp += ",";
        
        // Kiểm tra online status
        pthread_mutex_lock(&clients_mutex);
        bool is_online = username_to_socket.count(friends[i]) > 0;
        pthread_mutex_unlock(&clients_mutex);
        
        json_resp += "{\"username\":\"" + friends[i] + "\",\"online\":" + (is_online ? "true" : "false") + "}";
    }
    json_resp += "]}";
    
    send_packet(client_socket, S_RESP_FRIEND_LIST, STATUS_OK, json_resp);
}

void handle_pending_requests(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    vector<string> pending = db->getPendingFriendRequests(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Build JSON response
    string json_resp = "{\"pending\":[";
    for (size_t i = 0; i < pending.size(); i++) {
        if (i > 0) json_resp += ",";
        json_resp += "\"" + pending[i] + "\"";
    }
    json_resp += "]}";
    
    send_packet(client_socket, S_RESP_PENDING_REQUESTS, STATUS_OK, json_resp);
}

void handle_unfriend(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string friend_username = body.count("friend_username") ? body.at("friend_username") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["message"] = "Token không hợp lệ";
        send_packet(client_socket, S_RESP_UNFRIEND, STATUS_UNAUTHORIZED, JsonHelper::build(resp));
        return;
    }
    
    int friend_id = db->getUserId(friend_username);
    if (friend_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["message"] = "Không tìm thấy user";
        send_packet(client_socket, S_RESP_UNFRIEND, STATUS_NOT_FOUND, JsonHelper::build(resp));
        return;
    }
    
    // Sử dụng rejectFriendRequest để xóa friendship (cùng logic)
    bool success = db->rejectFriendRequest(user_id, friend_id);
    string my_username = db->getUsername(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    if (success) {
        map<string, string> resp;
        resp["message"] = "Đã hủy kết bạn với " + friend_username;
        send_packet(client_socket, S_RESP_UNFRIEND, STATUS_OK, JsonHelper::build(resp));
        cout << "✓ Unfriended: " << my_username << " <-> " << friend_username << endl;
    } else {
        map<string, string> resp;
        resp["message"] = "Không thể hủy kết bạn";
        send_packet(client_socket, S_RESP_UNFRIEND, STATUS_SERVER_ERROR, JsonHelper::build(resp));
    }
}

void handle_group_create(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string group_name = body.count("group_name") ? body.at("group_name") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Unauthorized";
        send_packet(client_socket, S_RESP_GROUP_CREATE, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
        return;
    }
    
    if (group_name.empty()) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Missing group_name";
        send_packet(client_socket, S_RESP_GROUP_CREATE, STATUS_BAD_REQUEST, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Create group
    int group_id = db->createGroup(group_name, user_id);
    if (group_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Failed to create group";
        send_packet(client_socket, S_RESP_GROUP_CREATE, STATUS_SERVER_ERROR, 
                   JsonHelper::build(resp));
        return;
    }
    
    string username = db->getUsername(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    map<string, string> resp;
    resp["group_id"] = to_string(group_id);
    resp["group_name"] = group_name;
    send_packet(client_socket, S_RESP_GROUP_CREATE, STATUS_CREATED, 
               JsonHelper::build(resp));
    
    cout << "✓ Group created: " << group_name << " by " << username << endl;
}

void handle_group_join(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string group_id_str = body.count("group_id") ? body.at("group_id") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int group_id = atoi(group_id_str.c_str());
    if (group_id <= 0) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Check if already member
    if (db->isGroupMember(group_id, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Add to group
    if (!db->addGroupMember(group_id, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string username = db->getUsername(user_id);
    string group_name = db->getGroupName(group_id);
    
    // Get all group members
    vector<int> member_ids = db->getGroupMembers(group_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Notify all members
    for (int member_id : member_ids) {
        pthread_mutex_lock(&db_mutex);
        string member_name = db->getUsername(member_id);
        pthread_mutex_unlock(&db_mutex);
        
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(member_name)) {
            int member_socket = username_to_socket[member_name];
            pthread_mutex_unlock(&clients_mutex);
            
            map<string, string> notify;
            notify["username"] = username;
            notify["group_id"] = group_id_str;
            send_packet(member_socket, S_NOTIFY_GROUP_JOIN, STATUS_OK, 
                       JsonHelper::build(notify));
        } else {
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    
    cout << "✓ User " << username << " joined group " << group_name << endl;
}

void handle_group_list(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Unauthorized";
        send_packet(client_socket, S_RESP_GROUP_LIST, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Get user's groups
    vector<map<string, string>> groups = db->getUserGroups(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Build response JSON với danh sách nhóm
    string json_resp = "{\"groups\":[";
    for (size_t i = 0; i < groups.size(); i++) {
        if (i > 0) json_resp += ",";
        json_resp += "{\"group_id\":\"" + groups[i]["group_id"] + "\",";
        json_resp += "\"group_name\":\"" + groups[i]["group_name"] + "\"}";
    }
    json_resp += "]}";
    
    send_packet(client_socket, S_RESP_GROUP_LIST, STATUS_OK, json_resp);
    
    cout << "✓ Sent group list to user_id " << user_id << " (" << groups.size() << " groups)" << endl;
}

void handle_all_groups(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Unauthorized";
        send_packet(client_socket, S_RESP_ALL_GROUPS, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Get all groups in system
    vector<map<string, string>> all_groups = db->getAllGroups();
    pthread_mutex_unlock(&db_mutex);
    
    // Build response JSON
    string json_resp = "{\"groups\":[";
    for (size_t i = 0; i < all_groups.size(); i++) {
        if (i > 0) json_resp += ",";
        json_resp += "{\"group_id\":\"" + all_groups[i]["group_id"] + "\",";
        json_resp += "\"group_name\":\"" + all_groups[i]["group_name"] + "\",";
        json_resp += "\"member_count\":\"" + all_groups[i]["member_count"] + "\"}";
    }
    json_resp += "]}";
    
    send_packet(client_socket, S_RESP_ALL_GROUPS, STATUS_OK, json_resp);
    
    cout << "✓ Sent all groups list to user_id " << user_id << " (" << all_groups.size() << " groups)" << endl;
}

/*
void handle_all_users(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["error"] = "Unauthorized";
        send_packet(client_socket, S_RESP_ALL_USERS, STATUS_UNAUTHORIZED, 
                   JsonHelper::build(resp));
        return;
    }
    
    // Get all users in system
    vector<map<string, string>> all_users = db->getAllUsers();
    pthread_mutex_unlock(&db_mutex);
    
    // Build response JSON
    string json_resp = "{\"users\":[";
    for (size_t i = 0; i < all_users.size(); i++) {
        if (i > 0) json_resp += ",";
        json_resp += "{\"user_id\":\"" + all_users[i]["user_id"] + "\",";
        json_resp += "\"username\":\"" + all_users[i]["username"] + "\",";
        json_resp += "\"is_online\":\"" + all_users[i]["is_online"] + "\"}";
    }
    json_resp += "]}";
    
    send_packet(client_socket, S_RESP_ALL_USERS, STATUS_OK, json_resp);
    
    cout << "✓ Sent all users list to user_id " << user_id << " (" << all_users.size() << " users)" << endl;
}
*/

void handle_group_leave(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string group_id_str = body.count("group_id") ? body.at("group_id") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int group_id = atoi(group_id_str.c_str());
    if (group_id <= 0) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Check if user is member
    if (!db->isGroupMember(group_id, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string username = db->getUsername(user_id);
    string group_name = db->getGroupName(group_id);
    
    // Get members before leaving (to notify them)
    vector<int> member_ids = db->getGroupMembers(group_id);
    
    // Remove user from group
    db->removeGroupMember(group_id, user_id);
    
    // Nếu là thành viên cuối cùng → xóa luôn nhóm
    if (member_ids.size() == 1) {
        db->deleteGroup(group_id);
        pthread_mutex_unlock(&db_mutex);
        cout << "✓ User " << username << " left and group " << group_name << " was deleted (no members left)" << endl;
        return;
    }
    
    pthread_mutex_unlock(&db_mutex);
    
    // Notify all remaining members
    for (int member_id : member_ids) {
        if (member_id == user_id) continue;  // Skip the leaving user
        
        pthread_mutex_lock(&db_mutex);
        string member_name = db->getUsername(member_id);
        pthread_mutex_unlock(&db_mutex);
        
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(member_name)) {
            int member_socket = username_to_socket[member_name];
            pthread_mutex_unlock(&clients_mutex);
            
            map<string, string> notify;
            notify["username"] = username;
            notify["group_id"] = group_id_str;
            send_packet(member_socket, S_NOTIFY_GROUP_LEAVE, STATUS_OK, 
                       JsonHelper::build(notify));
        } else {
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    
    cout << "✓ User " << username << " left group " << group_name << endl;
}

void handle_group_members(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string group_id_str = body.count("group_id") ? body.at("group_id") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        send_packet(client_socket, S_RESP_GROUP_MEMBERS, STATUS_UNAUTHORIZED, 
                   "{\"error\":\"Invalid token\"}");
        return;
    }
    
    int group_id = atoi(group_id_str.c_str());
    if (group_id <= 0) {
        pthread_mutex_unlock(&db_mutex);
        send_packet(client_socket, S_RESP_GROUP_MEMBERS, STATUS_BAD_REQUEST,
                   "{\"error\":\"Invalid group_id\"}");
        return;
    }
    
    // Check if user is member
    if (!db->isGroupMember(group_id, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        send_packet(client_socket, S_RESP_GROUP_MEMBERS, STATUS_FORBIDDEN,
                   "{\"error\":\"Not a member\"}");
        return;
    }
    
    // Get all members
    vector<int> member_ids = db->getGroupMembers(group_id);
    string group_name = db->getGroupName(group_id);
    
    // Build members list with online status
    string members_json = "[";
    for (size_t i = 0; i < member_ids.size(); i++) {
        string member_name = db->getUsername(member_ids[i]);
        bool is_online = db->isUserOnline(member_ids[i]);
        
        if (i > 0) members_json += ",";
        members_json += "{\"username\":\"" + member_name + "\",\"online\":" + (is_online ? "true" : "false") + "}";
    }
    members_json += "]";
    
    pthread_mutex_unlock(&db_mutex);
    
    // Build JSON manually to avoid escaping the members array
    string response_json = "{\"group_id\":\"" + group_id_str + 
                           "\",\"group_name\":\"" + group_name + 
                           "\",\"members\":" + members_json + "}";
    
    send_packet(client_socket, S_RESP_GROUP_MEMBERS, STATUS_OK, response_json);
    
    cout << "✓ Sent member list for group " << group_name << " (" << member_ids.size() << " members)" << endl;
}

void handle_msg_private(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string target_username = body.count("target_username") ? body.at("target_username") : "";
    string message = body.count("message") ? body.at("message") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string from_username = db->getUsername(user_id);
    int target_user_id = db->getUserId(target_username);
    
    if (target_user_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Save message to database
    db->savePrivateMessage(user_id, target_user_id, message);
    
    pthread_mutex_unlock(&db_mutex);
    
    // Send to target if online
    pthread_mutex_lock(&clients_mutex);
    if (username_to_socket.count(target_username)) {
        int target_socket = username_to_socket[target_username];
        pthread_mutex_unlock(&clients_mutex);
        
        map<string, string> notify;
        notify["from_username"] = from_username;
        notify["message"] = message;
        send_packet(target_socket, S_NOTIFY_MSG_PRIVATE, STATUS_OK, 
                   JsonHelper::build(notify));
        
        cout << "✓ Private message: " << from_username << " -> " << target_username << endl;
    } else {
        pthread_mutex_unlock(&clients_mutex);
        cout << "ℹ️  Message saved (user offline): " << from_username << " -> " << target_username << endl;
    }
}

void handle_msg_group(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string group_id_str = body.count("group_id") ? body.at("group_id") : "";
    string message = body.count("message") ? body.at("message") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int group_id = atoi(group_id_str.c_str());
    if (group_id <= 0) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Check if user is member
    if (!db->isGroupMember(group_id, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string from_username = db->getUsername(user_id);
    string group_name = db->getGroupName(group_id);
    
    // Save message
    db->saveGroupMessage(group_id, user_id, message);
    
    // Get all members
    vector<int> member_ids = db->getGroupMembers(group_id);
    pthread_mutex_unlock(&db_mutex);
    
    cout << "📤 Broadcasting to " << member_ids.size() << " members" << endl;
    
    // Broadcast to online members
    for (int member_id : member_ids) {
        pthread_mutex_lock(&db_mutex);
        string member_name = db->getUsername(member_id);
        pthread_mutex_unlock(&db_mutex);
        
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(member_name)) {
            int member_socket = username_to_socket[member_name];
            pthread_mutex_unlock(&clients_mutex);
            
            cout << "  → Sending to " << member_name << " (socket " << member_socket << ")" << endl;
            
            map<string, string> notify;
            notify["from_username"] = from_username;
            notify["group_id"] = group_id_str;
            notify["message"] = message;
            send_packet(member_socket, S_NOTIFY_MSG_GROUP, STATUS_OK, 
                       JsonHelper::build(notify));
        } else {
            cout << "  ✗ " << member_name << " not in online cache" << endl;
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    
    cout << "✓ Group message: " << from_username << " -> " << group_name << endl;
}

// ===== CHAT HISTORY HANDLERS =====

void handle_chat_history_private(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string target_username = body.count("target_username") ? body.at("target_username") : "";
    int offset = body.count("offset") ? atoi(body.at("offset").c_str()) : 0;
    int limit = body.count("limit") ? atoi(body.at("limit").c_str()) : 10;
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int target_user_id = db->getUserId(target_username);
    if (target_user_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string my_username = db->getUsername(user_id);
    
    // Get messages and total count
    vector<map<string, string>> messages = db->getPrivateMessages(user_id, target_user_id, limit, offset);
    int total_count = db->getPrivateMessageCount(user_id, target_user_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Build response JSON with is_read status
    string json = "{\"target_username\":\"" + target_username + "\",";
    json += "\"my_username\":\"" + my_username + "\",";
    json += "\"total_count\":" + to_string(total_count) + ",";
    json += "\"offset\":" + to_string(offset) + ",";
    json += "\"messages\":[";
    
    for (size_t i = 0; i < messages.size(); i++) {
        if (i > 0) json += ",";
        json += "{\"from_username\":\"" + messages[i]["from_username"] + "\",";
        json += "\"message\":\"" + JsonHelper::escapeJson(messages[i]["message_text"]) + "\",";
        json += "\"sent_at\":\"" + messages[i]["sent_at"] + "\",";
        json += "\"is_read\":\"" + messages[i]["is_read"] + "\"}";
    }
    json += "]}";
    
    send_packet(client_socket, S_RESP_CHAT_HISTORY_PRIVATE, STATUS_OK, json);
    cout << "✓ Sent private chat history: " << messages.size() << " messages (offset=" << offset << ")" << endl;
}

void handle_chat_history_group(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string group_id_str = body.count("group_id") ? body.at("group_id") : "";
    int offset = body.count("offset") ? atoi(body.at("offset").c_str()) : 0;
    int limit = body.count("limit") ? atoi(body.at("limit").c_str()) : 10;
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int group_id = atoi(group_id_str.c_str());
    if (group_id <= 0) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Check if user is member
    if (!db->isGroupMember(group_id, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    string group_name = db->getGroupName(group_id);
    
    // Get messages and total count
    vector<map<string, string>> messages = db->getGroupMessages(group_id, limit, offset);
    int total_count = db->getGroupMessageCount(group_id);
    pthread_mutex_unlock(&db_mutex);
    
    // Build response JSON
    string json = "{\"group_id\":\"" + group_id_str + "\",";
    json += "\"group_name\":\"" + group_name + "\",";
    json += "\"total_count\":" + to_string(total_count) + ",";
    json += "\"offset\":" + to_string(offset) + ",";
    json += "\"messages\":[";
    
    for (size_t i = 0; i < messages.size(); i++) {
        if (i > 0) json += ",";
        json += "{\"from_username\":\"" + messages[i]["from_username"] + "\",";
        json += "\"message\":\"" + JsonHelper::escapeJson(messages[i]["message_text"]) + "\",";
        json += "\"sent_at\":\"" + messages[i]["sent_at"] + "\"}";
    }
    json += "]}";
    
    send_packet(client_socket, S_RESP_CHAT_HISTORY_GROUP, STATUS_OK, json);
    cout << "✓ Sent group chat history: " << messages.size() << " messages (offset=" << offset << ")" << endl;
}

void handle_mark_messages_read(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string sender_username = body.count("from_username") ? body.at("from_username") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    int sender_id = db->getUserId(sender_username);
    if (sender_id == -1) {
        pthread_mutex_unlock(&db_mutex);
        return;
    }
    
    // Mark all messages from sender to current user as read
    bool updated = db->markAllMessagesAsRead(sender_id, user_id);
    pthread_mutex_unlock(&db_mutex);
    
    if (updated) {
        // Notify sender that their messages have been read
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(sender_username)) {
            int sender_socket = username_to_socket[sender_username];
            pthread_mutex_unlock(&clients_mutex);
            
            string my_username = "";
            pthread_mutex_lock(&db_mutex);
            my_username = db->getUsername(user_id);
            pthread_mutex_unlock(&db_mutex);
            
            map<string, string> notify;
            notify["reader_username"] = my_username;
            send_packet(sender_socket, S_NOTIFY_MESSAGES_READ, STATUS_OK, 
                       JsonHelper::build(notify));
            
            cout << "✓ Notified " << sender_username << " that messages were read by " << my_username << endl;
        } else {
            pthread_mutex_unlock(&clients_mutex);
        }
    }
}

// Base64 decode helper
string base64_decode(const string &in) {
    string out;
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

void handle_file_upload(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string fileName = body.count("file_name") ? body.at("file_name") : "";
    string fileSizeStr = body.count("file_size") ? body.at("file_size") : "0";
    string fileDataBase64 = body.count("file_data") ? body.at("file_data") : "";
    string target_username = body.count("target_username") ? body.at("target_username") : "";
    string group_id = body.count("group_id") ? body.at("group_id") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["message"] = "Invalid token";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_UNAUTHORIZED, JsonHelper::build(resp));
        return;
    }
    string sender_username = db->getUsername(user_id);
    pthread_mutex_unlock(&db_mutex);
    
    if (fileName.empty() || fileDataBase64.empty()) {
        map<string, string> resp;
        resp["message"] = "Missing file data";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_BAD_REQUEST, JsonHelper::build(resp));
        return;
    }
    
    // Decode base64
    string fileData = base64_decode(fileDataBase64);
    
    // Generate unique filename
    time_t now = time(nullptr);
    stringstream ss;
    ss << now << "_" << fileName;
    string savedFileName = ss.str();
    string filePath = "uploads/" + savedFileName;
    
    // Save file
    ofstream outFile(filePath, ios::binary);
    if (!outFile) {
        map<string, string> resp;
        resp["message"] = "Failed to save file";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_SERVER_ERROR, JsonHelper::build(resp));
        return;
    }
    outFile.write(fileData.c_str(), fileData.size());
    outFile.close();
    
    cout << "✓ Saved file: " << filePath << " (" << fileData.size() << " bytes)" << endl;
    
    // Send file message
    string fileMessage = "[FILE:" + savedFileName + "]" + fileName;
    
    if (!group_id.empty()) {
        // Group file
        pthread_mutex_lock(&db_mutex);
        int group_int_id = stoi(group_id);
        bool saved = db->saveGroupMessage(group_int_id, user_id, fileMessage);
        vector<int> member_ids = db->getGroupMembers(group_int_id);
        string groupName = db->getGroupName(group_int_id);
        
        // Convert member IDs to usernames
        vector<string> members;
        for (int member_id : member_ids) {
            string member_name = db->getUsername(member_id);
            if (!member_name.empty()) members.push_back(member_name);
        }
        pthread_mutex_unlock(&db_mutex);
        
        if (saved) {
            // Broadcast to group members
            pthread_mutex_lock(&clients_mutex);
            for (const string &member : members) {
                if (username_to_socket.count(member)) {
                    int mem_socket = username_to_socket[member];
                    map<string, string> notify;
                    notify["group_id"] = group_id;
                    notify["group_name"] = groupName;
                    notify["from_username"] = sender_username;
                    notify["message"] = fileMessage;
                    send_packet(mem_socket, S_NOTIFY_MSG_GROUP, STATUS_OK, JsonHelper::build(notify));
                }
            }
            pthread_mutex_unlock(&clients_mutex);
        }
    } else {
        // Private file
        pthread_mutex_lock(&db_mutex);
        int target_id = db->getUserId(target_username);
        bool saved = db->savePrivateMessage(user_id, target_id, fileMessage);
        pthread_mutex_unlock(&db_mutex);
        
        if (saved) {
            // Send to target if online
            pthread_mutex_lock(&clients_mutex);
            if (username_to_socket.count(target_username)) {
                int target_socket = username_to_socket[target_username];
                pthread_mutex_unlock(&clients_mutex);
                
                map<string, string> notify;
                notify["from_username"] = sender_username;
                notify["message"] = fileMessage;
                send_packet(target_socket, S_NOTIFY_MSG_PRIVATE, STATUS_OK, JsonHelper::build(notify));
            } else {
                pthread_mutex_unlock(&clients_mutex);
            }
        }
    }
    
    // Confirm to sender
    map<string, string> resp;
    resp["message"] = "File uploaded successfully";
    resp["file_name"] = savedFileName;
    send_packet(client_socket, S_RESP_FILE_OK, STATUS_OK, JsonHelper::build(resp));
}

void handle_file_download(int client_socket, const map<string, string>& body) {
    string token = body.count("token") ? body.at("token") : "";
    string fileName = body.count("file_name") ? body.at("file_name") : "";
    
    int user_id;
    pthread_mutex_lock(&db_mutex);
    if (!db->verifyToken(token, user_id)) {
        pthread_mutex_unlock(&db_mutex);
        map<string, string> resp;
        resp["message"] = "Invalid token";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_UNAUTHORIZED, JsonHelper::build(resp));
        return;
    }
    pthread_mutex_unlock(&db_mutex);
    
    if (fileName.empty()) {
        map<string, string> resp;
        resp["message"] = "Missing file name";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_BAD_REQUEST, JsonHelper::build(resp));
        return;
    }
    
    string filePath = "uploads/" + fileName;
    
    // Read file
    ifstream inFile(filePath, ios::binary);
    if (!inFile) {
        map<string, string> resp;
        resp["message"] = "File not found";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_NOT_FOUND, JsonHelper::build(resp));
        return;
    }
    
    // Get file size
    inFile.seekg(0, ios::end);
    streamsize fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);
    
    // Read file content
    vector<char> fileData(fileSize);
    if (!inFile.read(fileData.data(), fileSize)) {
        inFile.close();
        map<string, string> resp;
        resp["message"] = "Failed to read file";
        send_packet(client_socket, S_RESP_FILE_OK, STATUS_SERVER_ERROR, JsonHelper::build(resp));
        return;
    }
    inFile.close();
    
    // Encode to base64
    string fileDataStr(fileData.begin(), fileData.end());
    string fileDataBase64;
    
    // Simple base64 encoding
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int val = 0, valb = -6;
    for (unsigned char c : fileDataStr) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            fileDataBase64.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) fileDataBase64.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (fileDataBase64.size() % 4) fileDataBase64.push_back('=');
    
    // Send response
    map<string, string> resp;
    resp["message"] = "File download successful";
    resp["file_name"] = fileName;
    resp["file_size"] = to_string(fileSize);
    resp["file_data"] = fileDataBase64;
    
    send_packet(client_socket, S_RESP_FILE_OK, STATUS_OK, JsonHelper::build(resp));
    
    cout << "✓ Sent file download: " << fileName << " (" << fileSize << " bytes) to user_id " << user_id << endl;
}

// ===== CLIENT HANDLER =====

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    delete (int*)arg;
    
    cout << "✓ New client connected: socket " << client_socket << endl;
    
    while (true) {
        PacketHeader header;
        int bytes = recv(client_socket, &header, sizeof(PacketHeader), 0);
        
        if (bytes <= 0) {
            break;
        }
        
        // Read JSON body
        string json_body;
        if (header.body_length > 0) {
            char* buffer = new char[header.body_length + 1];
            recv(client_socket, buffer, header.body_length, 0);
            buffer[header.body_length] = '\0';
            json_body = string(buffer);
            delete[] buffer;
        }
        
        map<string, string> body = JsonHelper::parse(json_body);
        
        // Route command
        switch (header.command) {
            case C_REQ_REGISTER:
                handle_register(client_socket, body);
                break;
            case C_REQ_LOGIN:
                handle_login(client_socket, body);
                break;
            case C_REQ_CHANGE_PASS:
                handle_change_password(client_socket, body);
                break;
            case C_REQ_GROUP_CREATE:
                handle_group_create(client_socket, body);
                break;
            case C_REQ_GROUP_JOIN:
                handle_group_join(client_socket, body);
                break;
            case C_REQ_GROUP_LEAVE:
                handle_group_leave(client_socket, body);
                break;
            case C_REQ_GROUP_LIST:
                handle_group_list(client_socket, body);
                break;
            case C_REQ_ALL_GROUPS:
                handle_all_groups(client_socket, body);
                break;
            case C_REQ_GROUP_MEMBERS:
                handle_group_members(client_socket, body);
                break;
            case C_REQ_FRIEND_ADD:
                handle_friend_add(client_socket, body);
                break;
            case C_RESP_FRIEND_REQ:
                handle_friend_response(client_socket, body);
                break;
            case C_REQ_FRIEND_LIST:
                handle_friend_list(client_socket, body);
                break;
            // case C_REQ_ALL_USERS:
            //     handle_all_users(client_socket, body);
            //     break;
            case C_REQ_PENDING_REQUESTS:
                handle_pending_requests(client_socket, body);
                break;
            case C_REQ_UNFRIEND:
                handle_unfriend(client_socket, body);
                break;
            case C_REQ_MSG_PRIVATE:
                handle_msg_private(client_socket, body);
                break;
            case C_REQ_MSG_GROUP:
                handle_msg_group(client_socket, body);
                break;
            case C_REQ_CHAT_HISTORY_PRIVATE:
                handle_chat_history_private(client_socket, body);
                break;
            case C_REQ_CHAT_HISTORY_GROUP:
                handle_chat_history_group(client_socket, body);
                break;
            case C_REQ_MARK_MESSAGES_READ:
                handle_mark_messages_read(client_socket, body);
                break;
            case C_REQ_FILE_UPLOAD:
                handle_file_upload(client_socket, body);
                break;
            case C_REQ_FILE_DOWNLOAD:
                handle_file_download(client_socket, body);
                break;
            default:
                cout << "⚠ Unknown command: " << header.command << endl;
        }
    }
    
    // Cleanup on disconnect
    pthread_mutex_lock(&clients_mutex);
    int user_id = socket_to_userid.count(client_socket) ? socket_to_userid[client_socket] : -1;
    string username = socket_to_username.count(client_socket) ? socket_to_username[client_socket] : "";
    pthread_mutex_unlock(&clients_mutex);
    
    if (user_id != -1) {
        // Set user offline
        pthread_mutex_lock(&db_mutex);
        db->setUserOnline(user_id, false);
        vector<string> friends = db->getFriends(user_id);
        pthread_mutex_unlock(&db_mutex);
        
        // Notify friends
        for (const string& friend_name : friends) {
            pthread_mutex_lock(&clients_mutex);
            if (username_to_socket.count(friend_name)) {
                int friend_socket = username_to_socket[friend_name];
                pthread_mutex_unlock(&clients_mutex);
                
                map<string, string> notify;
                notify["username"] = username;
                send_packet(friend_socket, S_NOTIFY_FRIEND_OFFLINE, STATUS_OK, 
                           JsonHelper::build(notify));
            } else {
                pthread_mutex_unlock(&clients_mutex);
            }
        }
        
        // Remove from cache
        pthread_mutex_lock(&clients_mutex);
        username_to_socket.erase(username);
        socket_to_username.erase(client_socket);
        socket_to_userid.erase(client_socket);
        socket_to_token.erase(client_socket);
        pthread_mutex_unlock(&clients_mutex);
        
        cout << "✓ User logged out: " << username << endl;
    }
    
    close(client_socket);
    cout << "✓ Client disconnected: socket " << client_socket << endl;
    
    return NULL;
}

// ===== MAIN =====

int main(int argc, char* argv[]) {
    int port = (argc > 1) ? atoi(argv[1]) : 8888;
    
    cout << "==================================" << endl;
    cout << "   CHAT SERVER - CHECKPOINT 1" << endl;
    cout << "   với MySQL Database" << endl;
    cout << "==================================" << endl;
    
    // Connect to database
    db = new DBManager("localhost", "chat_user", "chat_password", "chat_app", 3306);
    if (!db->connect()) {
        cerr << "❌ Cannot connect to database" << endl;
        return 1;
    }
    
    // Clean expired sessions
    db->cleanExpiredSessions();
    
    // Reset all users to offline on server start
    db->resetAllUsersOffline();
    cout << "✓ Reset all users to offline" << endl;
    
    // Create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cerr << "❌ Cannot create socket" << endl;
        return 1;
    }
    
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "❌ Bind failed" << endl;
        return 1;
    }
    
    if (listen(server_socket, 10) < 0) {
        cerr << "❌ Listen failed" << endl;
        return 1;
    }
    
    cout << "✓ Server listening on port " << port << endl;
    cout << "==================================" << endl;
    
    // Accept connections
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            continue;
        }
        
        // Create thread for client
        pthread_t thread_id;
        int* sock_ptr = new int(client_socket);
        pthread_create(&thread_id, NULL, handle_client, sock_ptr);
        pthread_detach(thread_id);
    }
    
    close(server_socket);
    delete db;
    
    return 0;
}
