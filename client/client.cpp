#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <string>
#include <map>
#include <queue>
#include "../common/protocol.h"
#include "../common/json_helper.h"

using namespace std;

int client_socket;
string current_token;
string current_username;
atomic<bool> is_logged_in(false);
atomic<bool> running(true);

// ===== SYNCHRONIZATION FOR RESPONSE HANDLING =====
mutex response_mutex;
condition_variable response_cv;
queue<pair<PacketHeader, string>> response_queue;  // Queue để lưu responses

// Send packet (Header + JSON Body)
void send_packet(int command, const map<string, string>& body) {
    PacketHeader header(command, STATUS_OK);
    string json_body = JsonHelper::build(body);
    header.body_length = json_body.length();
    
    send(client_socket, &header, sizeof(PacketHeader), 0);
    if (header.body_length > 0) {
        send(client_socket, json_body.c_str(), json_body.length(), 0);
    }
}

// Đợi response từ server (được gọi bởi main thread)
pair<PacketHeader, string> wait_for_response() {
    unique_lock<mutex> lock(response_mutex);
    response_cv.wait(lock, []{ return !response_queue.empty() || !running; });
    
    if (!running || response_queue.empty()) {
        return {PacketHeader(), ""};
    }
    
    auto response = response_queue.front();
    response_queue.pop();
    return response;
}

// Kiểm tra command có phải là response (không phải notification)
bool is_response_command(int cmd) {
    return cmd == S_RESP_LOGIN || 
           cmd == S_RESP_REGISTER || 
           cmd == S_RESP_GROUP_CREATE ||
           cmd == S_RESP_GROUP_LIST ||
           cmd == S_RESP_FRIEND_LIST ||
           cmd == S_RESP_PENDING_REQUESTS ||
           cmd == S_RESP_UNFRIEND;
}

// Thread nhận tin nhắn từ server (CHỈ THREAD NÀY ĐỌC SOCKET)
void receive_thread() {
    while (running) {
        PacketHeader header;
        int bytes = recv(client_socket, &header, sizeof(PacketHeader), 0);
        
        if (bytes <= 0) {
            running = false;
            response_cv.notify_all();
            break;
        }
        
        string json_body;
        if (header.body_length > 0 && header.body_length < MAX_BODY_SIZE) {
            char* buffer = new char[header.body_length + 1];
            int total_read = 0;
            while (total_read < header.body_length) {
                int n = recv(client_socket, buffer + total_read, header.body_length - total_read, 0);
                if (n <= 0) break;
                total_read += n;
            }
            buffer[header.body_length] = '\0';
            json_body = string(buffer);
            delete[] buffer;
        }
        
        // Nếu là response → đẩy vào queue để main thread xử lý
        if (is_response_command(header.command)) {
            lock_guard<mutex> lock(response_mutex);
            response_queue.push({header, json_body});
            response_cv.notify_one();
            continue;
        }
        
        // Nếu là notification → xử lý ngay
        map<string, string> body = JsonHelper::parse(json_body);
        
        switch (header.command) {
            case S_NOTIFY_FRIEND_ONLINE:
                if (body.count("username")) {
                    cout << "\n🟢 " << body["username"] << " đang online" << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_FRIEND_OFFLINE:
                if (body.count("username")) {
                    cout << "\n🔴 " << body["username"] << " đã offline" << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_MSG_PRIVATE:
                if (body.count("from_username") && body.count("message")) {
                    cout << "\n💬 [" << body["from_username"] << "]: " 
                         << body["message"] << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_MSG_GROUP:
                if (body.count("from_username") && body.count("message") && body.count("group_id")) {
                    cout << "\n👥 [Group " << body["group_id"] << "] " 
                         << body["from_username"] << ": " << body["message"] << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_GROUP_JOIN:
                if (body.count("username") && body.count("group_id")) {
                    cout << "\n✅ " << body["username"] << " đã tham gia group " 
                         << body["group_id"] << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_GROUP_LEAVE:
                if (body.count("username") && body.count("group_id")) {
                    cout << "\n🚪 " << body["username"] << " đã rời khỏi group " 
                         << body["group_id"] << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_FRIEND_REQ:
                if (body.count("from_username")) {
                    cout << "\n📨 " << body["from_username"] << " muốn kết bạn với bạn!" << endl;
                    cout << "   (Vào menu 'Xem lời mời kết bạn' để phản hồi)" << endl;
                    cout << "> " << flush;
                }
                break;
                
            case S_NOTIFY_FRIEND_ACCEPT:
                if (body.count("username")) {
                    cout << "\n🤝 " << body["username"] << " đã chấp nhận lời mời kết bạn!" << endl;
                    cout << "> " << flush;
                }
                break;
                
            default:
                break;
        }
    }
}

void do_register() {
    string username, password;
    
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    
    map<string, string> body;
    body["username"] = username;
    body["pass_hash"] = password;
    
    send_packet(C_REQ_REGISTER, body);
    
    auto [header, json_resp] = wait_for_response();
    map<string, string> resp = JsonHelper::parse(json_resp);
    
    if (header.status == STATUS_CREATED) {
        cout << "✅ Đăng ký thành công!" << endl;
    } else {
        cout << "❌ Đăng ký thất bại: " << (resp.count("error") ? resp["error"] : "Unknown error") << endl;
    }
}

void do_login() {
    string username, password;
    
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    
    map<string, string> body;
    body["username"] = username;
    body["pass_hash"] = password;
    
    send_packet(C_REQ_LOGIN, body);
    
    auto [header, json_resp] = wait_for_response();
    map<string, string> resp = JsonHelper::parse(json_resp);
    
    if (header.status == STATUS_OK) {
        current_token = resp["token"];
        current_username = username;
        is_logged_in = true;
        cout << "✅ Đăng nhập thành công!" << endl;
        
        vector<string> friends = JsonHelper::parse_array(json_resp, "friends_online");
        if (!friends.empty()) {
            cout << "🟢 Bạn bè đang online: ";
            for (size_t i = 0; i < friends.size(); i++) {
                if (i > 0) cout << ", ";
                cout << friends[i];
            }
            cout << endl;
        }
    } else {
        cout << "❌ Đăng nhập thất bại: " << (resp.count("error") ? resp["error"] : "Unknown error") << endl;
    }
}

void do_create_group() {
    string group_name;
    
    cout << "Tên nhóm: ";
    cin.ignore();
    getline(cin, group_name);
    
    map<string, string> body;
    body["token"] = current_token;
    body["group_name"] = group_name;
    
    send_packet(C_REQ_GROUP_CREATE, body);
    
    auto [header, json_resp] = wait_for_response();
    map<string, string> resp = JsonHelper::parse(json_resp);
    
    if (header.status == STATUS_CREATED) {
        cout << "✅ Tạo nhóm thành công! Group ID: " << resp["group_id"] << endl;
    } else {
        cout << "❌ Tạo nhóm thất bại: " << (resp.count("error") ? resp["error"] : "Unknown error") << endl;
    }
}

void do_join_group() {
    string group_id;
    
    cout << "Group ID: ";
    cin >> group_id;
    
    map<string, string> body;
    body["token"] = current_token;
    body["group_id"] = group_id;
    
    send_packet(C_REQ_GROUP_JOIN, body);
    
    cout << "✅ Đã gửi yêu cầu tham gia nhóm" << endl;
}

void do_list_groups() {
    map<string, string> body;
    body["token"] = current_token;
    
    send_packet(C_REQ_GROUP_LIST, body);
    
    auto [header, json_resp] = wait_for_response();
    
    if (header.status == STATUS_OK) {
        cout << "\n📋 Danh sách nhóm đã tham gia:" << endl;
        cout << "─────────────────────────────" << endl;
        
        // Parse groups from JSON response
        // Format: {"groups":[{"group_id":"1","group_name":"test"},...]}
        size_t start = json_resp.find("[");
        size_t end = json_resp.find("]");
        
        if (start == string::npos || end == string::npos || start >= end) {
            cout << "  (Chưa tham gia nhóm nào)" << endl;
            return;
        }
        
        string groups_str = json_resp.substr(start + 1, end - start - 1);
        if (groups_str.empty() || groups_str == "") {
            cout << "  (Chưa tham gia nhóm nào)" << endl;
            return;
        }
        
        // Simple parsing
        size_t pos = 0;
        int count = 0;
        while ((pos = groups_str.find("group_id", pos)) != string::npos) {
            size_t id_start = groups_str.find("\"", pos + 10) + 1;
            size_t id_end = groups_str.find("\"", id_start);
            string gid = groups_str.substr(id_start, id_end - id_start);
            
            size_t name_pos = groups_str.find("group_name", pos);
            size_t name_start = groups_str.find("\"", name_pos + 12) + 1;
            size_t name_end = groups_str.find("\"", name_start);
            string gname = groups_str.substr(name_start, name_end - name_start);
            
            cout << "  ID: " << gid << " - " << gname << endl;
            count++;
            pos = name_end;
        }
        
        if (count == 0) {
            cout << "  (Chưa tham gia nhóm nào)" << endl;
        }
        cout << "─────────────────────────────" << endl;
    } else {
        cout << "❌ Không thể lấy danh sách nhóm" << endl;
    }
}

void do_send_private() {
    string target, message;
    
    cout << "Username người nhận: ";
    cin >> target;
    cout << "Tin nhắn: ";
    cin.ignore();
    getline(cin, message);
    
    map<string, string> body;
    body["token"] = current_token;
    body["target_username"] = target;
    body["message"] = message;
    
    send_packet(C_REQ_MSG_PRIVATE, body);
    
    cout << "✅ Đã gửi tin nhắn riêng tư" << endl;
}

void do_leave_group() {
    string group_id;
    
    cout << "Group ID cần rời: ";
    cin >> group_id;
    
    map<string, string> body;
    body["token"] = current_token;
    body["group_id"] = group_id;
    
    send_packet(C_REQ_GROUP_LEAVE, body);
    
    cout << "✅ Đã rời khỏi nhóm " << group_id << endl;
}

void do_send_group() {
    string group_id, message;
    
    cout << "Group ID: ";
    cin >> group_id;
    cout << "Tin nhắn: ";
    cin.ignore();
    getline(cin, message);
    
    map<string, string> body;
    body["token"] = current_token;
    body["group_id"] = group_id;
    body["message"] = message;
    
    send_packet(C_REQ_MSG_GROUP, body);
    
    cout << "✅ Đã gửi tin nhắn nhóm" << endl;
}

// ===== FRIEND FUNCTIONS =====

void do_add_friend() {
    string target;
    
    cout << "Username người muốn kết bạn: ";
    cin >> target;
    
    map<string, string> body;
    body["token"] = current_token;
    body["target_username"] = target;
    
    send_packet(C_REQ_FRIEND_ADD, body);
    
    cout << "✅ Đã gửi lời mời kết bạn đến " << target << endl;
}

void do_view_friend_requests() {
    map<string, string> body;
    body["token"] = current_token;
    
    send_packet(C_REQ_PENDING_REQUESTS, body);
    
    auto [header, json_resp] = wait_for_response();
    
    if (header.status == STATUS_OK) {
        cout << "\n📨 Danh sách lời mời kết bạn:" << endl;
        cout << "─────────────────────────────" << endl;
        
        // Parse pending list
        size_t start = json_resp.find("[");
        size_t end = json_resp.find("]");
        
        if (start == string::npos || end == string::npos || start >= end - 1) {
            cout << "  (Không có lời mời nào)" << endl;
            cout << "─────────────────────────────" << endl;
            return;
        }
        
        string list_str = json_resp.substr(start + 1, end - start - 1);
        if (list_str.empty()) {
            cout << "  (Không có lời mời nào)" << endl;
            cout << "─────────────────────────────" << endl;
            return;
        }
        
        // Parse usernames
        vector<string> pending;
        size_t pos = 0;
        while ((pos = list_str.find("\"", pos)) != string::npos) {
            size_t end_pos = list_str.find("\"", pos + 1);
            if (end_pos != string::npos) {
                string name = list_str.substr(pos + 1, end_pos - pos - 1);
                if (!name.empty() && name != ",") {
                    pending.push_back(name);
                }
                pos = end_pos + 1;
            } else {
                break;
            }
        }
        
        if (pending.empty()) {
            cout << "  (Không có lời mời nào)" << endl;
        } else {
            for (size_t i = 0; i < pending.size(); i++) {
                cout << "  " << (i+1) << ". " << pending[i] << endl;
            }
            cout << "─────────────────────────────" << endl;
            
            // Hỏi phản hồi
            cout << "\nNhập số thứ tự để phản hồi (0 để bỏ qua): ";
            int choice;
            cin >> choice;
            
            if (choice > 0 && choice <= (int)pending.size()) {
                string from_user = pending[choice - 1];
                cout << "1. Chấp nhận | 2. Từ chối: ";
                int action;
                cin >> action;
                
                map<string, string> resp_body;
                resp_body["token"] = current_token;
                resp_body["from_username"] = from_user;
                resp_body["action"] = (action == 1) ? "accept" : "reject";
                
                send_packet(C_RESP_FRIEND_REQ, resp_body);
                
                if (action == 1) {
                    cout << "✅ Đã chấp nhận lời mời từ " << from_user << endl;
                } else {
                    cout << "❌ Đã từ chối lời mời từ " << from_user << endl;
                }
            }
        }
    }
}

void do_view_friends() {
    map<string, string> body;
    body["token"] = current_token;
    
    send_packet(C_REQ_FRIEND_LIST, body);
    
    auto [header, json_resp] = wait_for_response();
    
    if (header.status == STATUS_OK) {
        cout << "\n👥 Danh sách bạn bè:" << endl;
        cout << "─────────────────────────────" << endl;
        
        // Parse friends list - now format: [{"username":"x","online":true}, ...]
        size_t start = json_resp.find("[");
        size_t end = json_resp.rfind("]");
        
        if (start == string::npos || end == string::npos || start >= end - 1) {
            cout << "  (Chưa có bạn bè)" << endl;
            cout << "─────────────────────────────" << endl;
            return;
        }
        
        string list_str = json_resp.substr(start + 1, end - start - 1);
        if (list_str.empty()) {
            cout << "  (Chưa có bạn bè)" << endl;
            cout << "─────────────────────────────" << endl;
            return;
        }
        
        // Parse each friend object và lưu vào vector
        vector<string> friend_names;
        vector<bool> friend_online;
        size_t pos = 0;
        while ((pos = list_str.find("{\"username\":\"", pos)) != string::npos) {
            pos += 13;  // Skip {"username":"
            size_t name_end = list_str.find("\"", pos);
            if (name_end == string::npos) break;
            
            string name = list_str.substr(pos, name_end - pos);
            
            // Find online status
            size_t online_pos = list_str.find("\"online\":", name_end);
            bool is_online = false;
            if (online_pos != string::npos) {
                is_online = (list_str.find("true", online_pos) == online_pos + 9);
            }
            
            if (!name.empty()) {
                friend_names.push_back(name);
                friend_online.push_back(is_online);
                string status = is_online ? "🟢 Online" : "⚫ Offline";
                cout << "  " << friend_names.size() << ". " << name << " - " << status << endl;
            }
            pos = name_end + 1;
        }
        
        if (friend_names.empty()) {
            cout << "  (Chưa có bạn bè)" << endl;
            cout << "─────────────────────────────" << endl;
            return;
        }
        
        cout << "─────────────────────────────" << endl;
        cout << "Chọn bạn bè (0 để quay lại): ";
        int choice;
        cin >> choice;
        
        if (choice > 0 && choice <= (int)friend_names.size()) {
            string selected_friend = friend_names[choice - 1];
            bool is_online = friend_online[choice - 1];
            
            cout << "\n📌 Đã chọn: " << selected_friend;
            cout << (is_online ? " (🟢 Online)" : " (⚫ Offline)") << endl;
            cout << "─────────────────────────────" << endl;
            cout << "  1. 💬 Nhắn tin" << endl;
            cout << "  2. ❌ Hủy kết bạn" << endl;
            cout << "  0. Quay lại" << endl;
            cout << "─────────────────────────────" << endl;
            cout << "Chọn: ";
            
            int action;
            cin >> action;
            
            if (action == 1) {
                // Nhắn tin cho bạn bè
                cout << "Nhập tin nhắn: ";
                cin.ignore();
                string message;
                getline(cin, message);
                
                map<string, string> msg_body;
                msg_body["token"] = current_token;
                msg_body["target_username"] = selected_friend;
                msg_body["message"] = message;
                
                send_packet(C_REQ_MSG_PRIVATE, msg_body);
                cout << "✅ Đã gửi tin nhắn đến " << selected_friend << endl;
                
            } else if (action == 2) {
                // Hủy kết bạn
                cout << "Bạn có chắc muốn hủy kết bạn với " << selected_friend << "? (y/n): ";
                char confirm;
                cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    map<string, string> unfriend_body;
                    unfriend_body["token"] = current_token;
                    unfriend_body["friend_username"] = selected_friend;
                    
                    send_packet(C_REQ_UNFRIEND, unfriend_body);
                    
                    auto [resp_header, resp_body] = wait_for_response();
                    auto resp = JsonHelper::parse(resp_body);
                    string message = resp.count("message") ? resp.at("message") : "";
                    
                    if (resp_header.status == STATUS_OK) {
                        cout << "✅ " << message << endl;
                    } else {
                        cout << "❌ " << message << endl;
                    }
                } else {
                    cout << "Đã hủy thao tác." << endl;
                }
            }
        }
    }
}

void show_menu() {
    cout << "\n========================================" << endl;
    if (is_logged_in) {
        cout << "  User: " << current_username << endl;
        cout << "========================================" << endl;
        cout << "  --- NHÓM ---" << endl;
        cout << "  1. Tạo nhóm" << endl;
        cout << "  2. Tham gia nhóm" << endl;
        cout << "  3. Rời nhóm" << endl;
        cout << "  4. Xem danh sách nhóm" << endl;
        cout << "  --- BẠN BÈ ---" << endl;
        cout << "  5. Gửi lời mời kết bạn" << endl;
        cout << "  6. Xem lời mời kết bạn" << endl;
        cout << "  7. Xem danh sách bạn bè (có thể hủy kết bạn)" << endl;
        cout << "  --- CHAT ---" << endl;
        cout << "  8. Gửi tin nhắn riêng" << endl;
        cout << "  9. Gửi tin nhắn nhóm" << endl;
        cout << "  0. Thoát" << endl;
    } else {
        cout << "  CHAT CLIENT " << endl;
        cout << "========================================" << endl;
        cout << "  1. Đăng ký" << endl;
        cout << "  2. Đăng nhập" << endl;
        cout << "  0. Thoát" << endl;
    }
    cout << "========================================" << endl;
}

int main(int argc, char* argv[]) {
    string server_ip = (argc > 1) ? argv[1] : "127.0.0.1";
    int port = (argc > 2) ? atoi(argv[2]) : 8888;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        cerr << "❌ Không thể tạo socket" << endl;
        return 1;
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);
    
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "❌ Không thể kết nối đến server " << server_ip << ":" << port << endl;
        return 1;
    }
    
    cout << "✅ Đã kết nối đến server!" << endl;
    
    // Start receive thread - CHỈ THREAD NÀY ĐỌC SOCKET
    thread recv_thread(receive_thread);
    recv_thread.detach();
    
    while (running) {
        show_menu();
        
        cout << "> ";
        int choice;
        cin >> choice;
        
        if (!is_logged_in) {
            switch (choice) {
                case 1:
                    do_register();
                    break;
                case 2:
                    do_login();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    cout << "❌ Lựa chọn không hợp lệ" << endl;
            }
        } else {
            switch (choice) {
                case 1:
                    do_create_group();
                    break;
                case 2:
                    do_join_group();
                    break;
                case 3:
                    do_leave_group();
                    break;
                case 4:
                    do_list_groups();
                    break;
                case 5:
                    do_add_friend();
                    break;
                case 6:
                    do_view_friend_requests();
                    break;
                case 7:
                    do_view_friends();
                    break;
                case 8:
                    do_send_private();
                    break;
                case 9:
                    do_send_group();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    cout << "❌ Lựa chọn không hợp lệ" << endl;
            }
        }
    }
    
    close(client_socket);
    cout << "👋 Tạm biệt!" << endl;
    
    return 0;
}
