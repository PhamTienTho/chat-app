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
           cmd == S_RESP_GROUP_CREATE;
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

void show_menu() {
    cout << "\n========================================" << endl;
    if (is_logged_in) {
        cout << "  User: " << current_username << endl;
        cout << "========================================" << endl;
        cout << "  1. Tạo nhóm" << endl;
        cout << "  2. Tham gia nhóm" << endl;
        cout << "  3. Gửi tin nhắn riêng" << endl;
        cout << "  4. Gửi tin nhắn nhóm" << endl;
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
                    do_send_private();
                    break;
                case 4:
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
