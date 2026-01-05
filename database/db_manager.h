/*
 * DATABASE MANAGER
 * Quản lý kết nối và thao tác với MySQL database
 */

#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class DBManager {
private:
    MYSQL* conn;
    string host;
    string user;
    string password;
    string database;
    int port;
    
public:
    DBManager(const string& host = "localhost", 
              const string& user = "root",
              const string& password = "",
              const string& database = "chat_app",
              int port = 3306);
    
    ~DBManager();
    
    // Connection management
    bool connect();
    void disconnect();
    bool isConnected();
    
    // User operations
    bool createUser(const string& username, const string& password_hash);
    bool verifyUser(const string& username, const string& password_hash);
    int getUserId(const string& username);
    string getUsername(int user_id);
    bool setUserOnline(int user_id, bool is_online);
    bool updateLastLogin(int user_id);
    
    // Session/Token operations
    string createSession(int user_id);
    bool verifyToken(const string& token, int& user_id);
    bool deleteSession(const string& token);
    void cleanExpiredSessions();
    
    // Friendship operations
    bool sendFriendRequest(int requester_id, int target_user_id);
    bool acceptFriendRequest(int user_id1, int user_id2);
    bool rejectFriendRequest(int user_id1, int user_id2);
    vector<string> getFriends(int user_id);
    vector<string> getOnlineFriends(int user_id);
    vector<string> getPendingFriendRequests(int user_id);
    bool areFriends(int user_id1, int user_id2);
    
    // Group operations
    int createGroup(const string& group_name, int creator_id);
    bool addGroupMember(int group_id, int user_id, const string& role = "member");
    bool removeGroupMember(int group_id, int user_id);
    bool isGroupMember(int group_id, int user_id);
    vector<int> getGroupMembers(int group_id);
    vector<map<string, string>> getUserGroups(int user_id);
    string getGroupName(int group_id);
    
    // Message operations
    bool savePrivateMessage(int from_user_id, int to_user_id, const string& message);
    bool saveGroupMessage(int group_id, int from_user_id, const string& message);
    vector<map<string, string>> getPrivateMessages(int user_id1, int user_id2, int limit = 50);
    vector<map<string, string>> getGroupMessages(int group_id, int limit = 50);
    bool markMessageAsRead(int message_id);
    
    // Utility
    string escapeString(const string& str);
    void printError();
};

#endif // DB_MANAGER_H
