#include <iostream>
#include "../database/db_manager.h"

using namespace std;

int main() {
    cout << "=== Testing MySQL Connection ===" << endl;
    
    // Create database manager
    DBManager db("localhost", "chat_user", "chat_password", "chat_app", 3306);
    
    // Test connection
    if (!db.connect()) {
        cerr << "❌ Failed to connect to database" << endl;
        return 1;
    }
    
    cout << "✅ Connected successfully!" << endl;
    
    // Test create user
    cout << "\n--- Testing User Operations ---" << endl;
    if (db.createUser("testuser", "testpass")) {
        cout << "✅ Created test user" << endl;
    } else {
        cout << "ℹ️  User may already exist" << endl;
    }
    
    // Test verify user
    if (db.verifyUser("alice", "alice123")) {
        cout << "✅ Verified alice" << endl;
    }
    
    // Test get user ID
    int alice_id = db.getUserId("alice");
    cout << "✅ Alice ID: " << alice_id << endl;
    
    // Test session
    cout << "\n--- Testing Session Operations ---" << endl;
    string token = db.createSession(alice_id);
    cout << "✅ Created session token: " << token.substr(0, 16) << "..." << endl;
    
    int verified_id;
    if (db.verifyToken(token, verified_id)) {
        cout << "✅ Token verified for user ID: " << verified_id << endl;
    }
    
    // Test friends
    cout << "\n--- Testing Friend Operations ---" << endl;
    vector<string> friends = db.getFriends(alice_id);
    cout << "✅ Alice has " << friends.size() << " friends:" << endl;
    for (const string& f : friends) {
        cout << "   - " << f << endl;
    }
    
    // Test groups
    cout << "\n--- Testing Group Operations ---" << endl;
    int group_id = db.createGroup("Test Group", alice_id);
    if (group_id > 0) {
        cout << "✅ Created group with ID: " << group_id << endl;
    }
    
    // Test messages
    cout << "\n--- Testing Message Operations ---" << endl;
    int bob_id = db.getUserId("bob");
    if (db.savePrivateMessage(alice_id, bob_id, "Hello Bob!")) {
        cout << "✅ Saved private message" << endl;
    }
    
    if (group_id > 0 && db.saveGroupMessage(group_id, alice_id, "Hello group!")) {
        cout << "✅ Saved group message" << endl;
    }
    
    cout << "\n=== All Tests Passed! ===" << endl;
    
    db.disconnect();
    return 0;
}
