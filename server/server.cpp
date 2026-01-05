#include <iostream>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <string>
#include <vector>
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
    
    // Broadcast to online members
    for (int member_id : member_ids) {
        pthread_mutex_lock(&db_mutex);
        string member_name = db->getUsername(member_id);
        pthread_mutex_unlock(&db_mutex);
        
        pthread_mutex_lock(&clients_mutex);
        if (username_to_socket.count(member_name)) {
            int member_socket = username_to_socket[member_name];
            pthread_mutex_unlock(&clients_mutex);
            
            map<string, string> notify;
            notify["from_username"] = from_username;
            notify["group_id"] = group_id_str;
            notify["message"] = message;
            send_packet(member_socket, S_NOTIFY_MSG_GROUP, STATUS_OK, 
                       JsonHelper::build(notify));
        } else {
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    
    cout << "✓ Group message: " << from_username << " -> " << group_name << endl;
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
            case C_REQ_GROUP_CREATE:
                handle_group_create(client_socket, body);
                break;
            case C_REQ_GROUP_JOIN:
                handle_group_join(client_socket, body);
                break;
            case C_REQ_MSG_PRIVATE:
                handle_msg_private(client_socket, body);
                break;
            case C_REQ_MSG_GROUP:
                handle_msg_group(client_socket, body);
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
