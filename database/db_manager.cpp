/*
 * DATABASE MANAGER IMPLEMENTATION
 */

#include "db_manager.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>

DBManager::DBManager(const string& host, const string& user, 
                     const string& password, const string& database, int port)
    : conn(nullptr), host(host), user(user), password(password), 
      database(database), port(port) {
}

DBManager::~DBManager() {
    disconnect();
}

bool DBManager::connect() {
    conn = mysql_init(nullptr);
    if (!conn) {
        cerr << "❌ MySQL init failed" << endl;
        return false;
    }
    
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), 
                           password.c_str(), database.c_str(), 
                           port, nullptr, 0)) {
        printError();
        return false;
    }
    
    // Set UTF-8 encoding
    mysql_set_character_set(conn, "utf8mb4");
    
    cout << "✓ Connected to MySQL database: " << database << endl;
    return true;
}

void DBManager::disconnect() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
    }
}

bool DBManager::isConnected() {
    return conn != nullptr && mysql_ping(conn) == 0;
}

void DBManager::printError() {
    if (conn) {
        cerr << "❌ MySQL Error: " << mysql_error(conn) << endl;
    }
}

string DBManager::escapeString(const string& str) {
    if (!conn) return str;
    
    char* buffer = new char[str.length() * 2 + 1];
    mysql_real_escape_string(conn, buffer, str.c_str(), str.length());
    string result(buffer);
    delete[] buffer;
    return result;
}

// ===== USER OPERATIONS =====

bool DBManager::createUser(const string& username, const string& password_hash) {
    string query = "INSERT INTO users (username, password_hash) VALUES ('" +
                   escapeString(username) + "', '" + 
                   escapeString(password_hash) + "')";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::verifyUser(const string& username, const string& password_hash) {
    string query = "SELECT user_id FROM users WHERE username='" +
                   escapeString(username) + "' AND password_hash='" +
                   escapeString(password_hash) + "'";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    bool exists = (mysql_num_rows(result) > 0);
    mysql_free_result(result);
    return exists;
}

int DBManager::getUserId(const string& username) {
    string query = "SELECT user_id FROM users WHERE username='" +
                   escapeString(username) + "'";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return -1;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        return -1;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int user_id = atoi(row[0]);
    mysql_free_result(result);
    return user_id;
}

string DBManager::getUsername(int user_id) {
    string query = "SELECT username FROM users WHERE user_id=" + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return "";
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        return "";
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    string username = row[0];
    mysql_free_result(result);
    return username;
}

bool DBManager::setUserOnline(int user_id, bool is_online) {
    string query = "UPDATE users SET is_online=" + string(is_online ? "1" : "0") +
                   " WHERE user_id=" + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::isUserOnline(int user_id) {
    string query = "SELECT is_online FROM users WHERE user_id=" + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) return false;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    bool online = (row && row[0] && string(row[0]) == "1");
    mysql_free_result(result);
    return online;
}

void DBManager::resetAllUsersOffline() {
    string query = "UPDATE users SET is_online=0";
    if (mysql_query(conn, query.c_str())) {
        printError();
    }
}

bool DBManager::updateLastLogin(int user_id) {
    string query = "UPDATE users SET last_login=NOW() WHERE user_id=" + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::changePassword(int user_id, const string& old_password, const string& new_password) {
    // First verify old password
    string verify_query = "SELECT user_id FROM users WHERE user_id=" + to_string(user_id) +
                         " AND password_hash='" + escapeString(old_password) + "'";
    
    if (mysql_query(conn, verify_query.c_str())) {
        printError();
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        return false;  // Old password is incorrect
    }
    mysql_free_result(result);
    
    // Update to new password
    string update_query = "UPDATE users SET password_hash='" + escapeString(new_password) +
                         "' WHERE user_id=" + to_string(user_id);
    
    if (mysql_query(conn, update_query.c_str())) {
        printError();
        return false;
    }
    return true;
}

// ===== SESSION OPERATIONS =====

string DBManager::createSession(int user_id) {
    // Generate random token
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 15);
    const char* hex = "0123456789abcdef";
    
    string token;
    for (int i = 0; i < 64; i++) {
        token += hex[dis(gen)];
    }
    
    // Delete old sessions for this user
    string del_query = "DELETE FROM sessions WHERE user_id=" + to_string(user_id);
    mysql_query(conn, del_query.c_str());
    
    // Create new session (expires in 24 hours)
    string query = "INSERT INTO sessions (user_id, token, expires_at) VALUES (" +
                   to_string(user_id) + ", '" + token + "', DATE_ADD(NOW(), INTERVAL 24 HOUR))";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return "";
    }
    
    return token;
}

bool DBManager::verifyToken(const string& token, int& user_id) {
    string query = "SELECT user_id FROM sessions WHERE token='" +
                   escapeString(token) + "' AND expires_at > NOW()";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        return false;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    user_id = atoi(row[0]);
    mysql_free_result(result);
    return true;
}

bool DBManager::deleteSession(const string& token) {
    string query = "DELETE FROM sessions WHERE token='" + escapeString(token) + "'";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

void DBManager::cleanExpiredSessions() {
    mysql_query(conn, "DELETE FROM sessions WHERE expires_at < NOW()");
}

// ===== FRIENDSHIP OPERATIONS =====

bool DBManager::sendFriendRequest(int requester_id, int target_user_id) {
    // Ensure user_id1 < user_id2 for consistency
    int user_id1 = min(requester_id, target_user_id);
    int user_id2 = max(requester_id, target_user_id);
    
    string query = "INSERT INTO friendships (user_id1, user_id2, requester_id, status) VALUES (" +
                   to_string(user_id1) + ", " + to_string(user_id2) + ", " +
                   to_string(requester_id) + ", 'pending')";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::acceptFriendRequest(int user_id1, int user_id2) {
    int uid1 = min(user_id1, user_id2);
    int uid2 = max(user_id1, user_id2);
    
    string query = "UPDATE friendships SET status='accepted' WHERE user_id1=" +
                   to_string(uid1) + " AND user_id2=" + to_string(uid2);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::rejectFriendRequest(int user_id1, int user_id2) {
    int uid1 = min(user_id1, user_id2);
    int uid2 = max(user_id1, user_id2);
    
    string query = "DELETE FROM friendships WHERE user_id1=" +
                   to_string(uid1) + " AND user_id2=" + to_string(uid2);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

vector<string> DBManager::getFriends(int user_id) {
    // Query trực tiếp từ friendships table thay vì dùng view bị lỗi
    string query = "SELECT u2.username FROM friendships f "
                   "JOIN users u2 ON f.user_id2 = u2.user_id "
                   "WHERE f.user_id1 = " + to_string(user_id) + " AND f.status = 'accepted' "
                   "UNION "
                   "SELECT u1.username FROM friendships f "
                   "JOIN users u1 ON f.user_id1 = u1.user_id "
                   "WHERE f.user_id2 = " + to_string(user_id) + " AND f.status = 'accepted'";
    
    vector<string> friends;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return friends;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        friends.push_back(row[0]);
    }
    mysql_free_result(result);
    return friends;
}

vector<string> DBManager::getOnlineFriends(int user_id) {
    string query = "SELECT CASE WHEN f.user_id1=" + to_string(user_id) + 
                   " THEN u2.username ELSE u1.username END AS friend_username "
                   "FROM friendships f "
                   "JOIN users u1 ON f.user_id1=u1.user_id "
                   "JOIN users u2 ON f.user_id2=u2.user_id "
                   "WHERE (f.user_id1=" + to_string(user_id) + " OR f.user_id2=" + to_string(user_id) + ") "
                   "AND f.status='accepted' "
                   "AND (CASE WHEN f.user_id1=" + to_string(user_id) + " THEN u2.is_online ELSE u1.is_online END)=1";
    
    vector<string> friends;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return friends;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        friends.push_back(row[0]);
    }
    mysql_free_result(result);
    return friends;
}

vector<string> DBManager::getPendingFriendRequests(int user_id) {
    string query = "SELECT u.username FROM friendships f "
                   "JOIN users u ON f.requester_id=u.user_id "
                   "WHERE (f.user_id1=" + to_string(user_id) + " OR f.user_id2=" + to_string(user_id) + ") "
                   "AND f.requester_id!=" + to_string(user_id) + " "
                   "AND f.status='pending'";
    
    vector<string> requests;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return requests;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        requests.push_back(row[0]);
    }
    mysql_free_result(result);
    return requests;
}

bool DBManager::areFriends(int user_id1, int user_id2) {
    int uid1 = min(user_id1, user_id2);
    int uid2 = max(user_id1, user_id2);
    
    string query = "SELECT 1 FROM friendships WHERE user_id1=" + to_string(uid1) +
                   " AND user_id2=" + to_string(uid2) + " AND status='accepted'";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    bool friends = (mysql_num_rows(result) > 0);
    mysql_free_result(result);
    return friends;
}

// ===== GROUP OPERATIONS =====

int DBManager::createGroup(const string& group_name, int creator_id) {
    string query = "INSERT INTO `groups` (group_name, creator_id) VALUES ('" +
                   escapeString(group_name) + "', " + to_string(creator_id) + ")";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return -1;
    }
    
    int group_id = mysql_insert_id(conn);
    
    // Add creator as admin
    addGroupMember(group_id, creator_id, "admin");
    
    return group_id;
}

bool DBManager::addGroupMember(int group_id, int user_id, const string& role) {
    string query = "INSERT INTO group_members (group_id, user_id, role) VALUES (" +
                   to_string(group_id) + ", " + to_string(user_id) + ", '" + role + "')";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::removeGroupMember(int group_id, int user_id) {
    string query = "DELETE FROM group_members WHERE group_id=" + to_string(group_id) +
                   " AND user_id=" + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::deleteGroup(int group_id) {
    // Xóa tất cả members trước (nếu còn)
    string query1 = "DELETE FROM group_members WHERE group_id=" + to_string(group_id);
    mysql_query(conn, query1.c_str());
    
    // Xóa tin nhắn trong nhóm
    string query2 = "DELETE FROM group_messages WHERE group_id=" + to_string(group_id);
    mysql_query(conn, query2.c_str());
    
    // Xóa nhóm
    string query3 = "DELETE FROM `groups` WHERE group_id=" + to_string(group_id);
    if (mysql_query(conn, query3.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::isGroupMember(int group_id, int user_id) {
    string query = "SELECT 1 FROM group_members WHERE group_id=" + to_string(group_id) +
                   " AND user_id=" + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    bool is_member = (mysql_num_rows(result) > 0);
    mysql_free_result(result);
    return is_member;
}

vector<int> DBManager::getGroupMembers(int group_id) {
    string query = "SELECT user_id FROM group_members WHERE group_id=" + to_string(group_id);
    
    vector<int> members;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return members;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        members.push_back(atoi(row[0]));
    }
    mysql_free_result(result);
    return members;
}

vector<map<string, string>> DBManager::getUserGroups(int user_id) {
    string query = "SELECT g.group_id, g.group_name FROM `groups` g "
                   "JOIN group_members gm ON g.group_id=gm.group_id "
                   "WHERE gm.user_id=" + to_string(user_id);
    
    vector<map<string, string>> groups;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return groups;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        map<string, string> group;
        group["group_id"] = row[0];
        group["group_name"] = row[1];
        groups.push_back(group);
    }
    mysql_free_result(result);
    return groups;
}

string DBManager::getGroupName(int group_id) {
    string query = "SELECT group_name FROM `groups` WHERE group_id=" + to_string(group_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return "";
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        return "";
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    string name = row[0];
    mysql_free_result(result);
    return name;
}

// ===== MESSAGE OPERATIONS =====

bool DBManager::savePrivateMessage(int from_user_id, int to_user_id, const string& message) {
    string query = "INSERT INTO private_messages (from_user_id, to_user_id, message_text) VALUES (" +
                   to_string(from_user_id) + ", " + to_string(to_user_id) + ", '" +
                   escapeString(message) + "')";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

bool DBManager::saveGroupMessage(int group_id, int from_user_id, const string& message) {
    string query = "INSERT INTO group_messages (group_id, from_user_id, message_text) VALUES (" +
                   to_string(group_id) + ", " + to_string(from_user_id) + ", '" +
                   escapeString(message) + "')";
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}

vector<map<string, string>> DBManager::getPrivateMessages(int user_id1, int user_id2, int limit) {
    string query = "SELECT m.message_id, u.username AS from_username, m.message_text, m.sent_at "
                   "FROM private_messages m "
                   "JOIN users u ON m.from_user_id=u.user_id "
                   "WHERE (m.from_user_id=" + to_string(user_id1) + " AND m.to_user_id=" + to_string(user_id2) + ") "
                   "OR (m.from_user_id=" + to_string(user_id2) + " AND m.to_user_id=" + to_string(user_id1) + ") "
                   "ORDER BY m.sent_at DESC LIMIT " + to_string(limit);
    
    vector<map<string, string>> messages;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return messages;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        map<string, string> msg;
        msg["message_id"] = row[0];
        msg["from_username"] = row[1];
        msg["message_text"] = row[2];
        msg["sent_at"] = row[3];
        messages.push_back(msg);
    }
    mysql_free_result(result);
    return messages;
}

vector<map<string, string>> DBManager::getGroupMessages(int group_id, int limit) {
    string query = "SELECT m.message_id, u.username AS from_username, m.message_text, m.sent_at "
                   "FROM group_messages m "
                   "JOIN users u ON m.from_user_id=u.user_id "
                   "WHERE m.group_id=" + to_string(group_id) + " "
                   "ORDER BY m.sent_at DESC LIMIT " + to_string(limit);
    
    vector<map<string, string>> messages;
    if (mysql_query(conn, query.c_str())) {
        printError();
        return messages;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        map<string, string> msg;
        msg["message_id"] = row[0];
        msg["from_username"] = row[1];
        msg["message_text"] = row[2];
        msg["sent_at"] = row[3];
        messages.push_back(msg);
    }
    mysql_free_result(result);
    return messages;
}

bool DBManager::markMessageAsRead(int message_id) {
    string query = "UPDATE private_messages SET is_read=1 WHERE message_id=" + to_string(message_id);
    
    if (mysql_query(conn, query.c_str())) {
        printError();
        return false;
    }
    return true;
}
