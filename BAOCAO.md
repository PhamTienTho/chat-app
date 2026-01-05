# BÁO CÁO ĐỒ ÁN
## ỨNG DỤNG CHAT TRỰC TUYẾN - CHECKPOINT 1

**Nhóm:** 11  
**Ngày báo cáo:** 10/12/2025

---

## 1. TỔNG QUAN DỰ ÁN

### 1.1. Mục tiêu
Xây dựng hệ thống chat trực tuyến client-server sử dụng:
- Socket TCP/IP để kết nối
- Giao thức tầng ứng dụng Header + JSON
- MySQL database để lưu trữ dữ liệu
- Multi-threading để xử lý đồng thời nhiều client

### 1.2. Công nghệ sử dụng
- **Ngôn ngữ:** C++17
- **Socket:** POSIX socket (sys/socket.h)
- **Threading:** pthread
- **Database:** MySQL 8.0 với thư viện mysqlclient
- **Protocol:** Custom binary header (20 bytes) + JSON body
- **Build tool:** Make

---

## 2. KIẾN TRÚC HỆ THỐNG

### 2.1. Mô hình Client-Server

```
┌─────────────┐         ┌─────────────┐         ┌─────────────┐
│   Client 1  │◄───────►│             │         │             │
│   (Alice)   │         │   Server    │◄───────►│   MySQL     │
└─────────────┘         │  (pthread)  │         │  Database   │
                        │             │         │             │
┌─────────────┐         │   Port 8888 │         └─────────────┘
│   Client 2  │◄───────►│             │
│    (Bob)    │         └─────────────┘
└─────────────┘
```

### 2.2. Cấu trúc thư mục

```
chat_app/
├── common/
│   ├── protocol.h          # Định nghĩa protocol và command IDs
│   └── json_helper.h       # Hàm parse/build JSON
├── database/
│   ├── schema.sql          # Database schema
│   ├── db_manager.h        # Database manager header
│   ├── db_manager.cpp      # Database manager implementation
│   └── SETUP_MYSQL.md      # Hướng dẫn cài đặt MySQL
├── server/
│   ├── server.cpp          # Server implementation
│   └── Makefile            # Build script cho server
├── client/
│   ├── client.cpp          # Client implementation
│   └── Makefile            # Build script cho client
└── README.md
```

---

## 3. GIAO THỨC TẦNG ỨNG DỤNG

### 3.1. Cấu trúc gói tin

**Header (20 bytes cố định):**
```cpp
struct PacketHeader {
    int command;       // 4 bytes - Mã lệnh
    int status;        // 4 bytes - Mã trạng thái (HTTP-like)
    long timestamp;    // 8 bytes - Thời gian gửi
    int body_length;   // 4 bytes - Độ dài JSON body
};
```

**Body (Variable length):**
- Format: JSON string
- Max size: 64KB
- Encoding: UTF-8

### 3.2. Danh sách Commands đã implement

#### **Nhóm xác thực (1xx):**
- `101 - C_REQ_LOGIN`: Client yêu cầu đăng nhập
- `102 - S_RESP_LOGIN`: Server phản hồi đăng nhập
- `103 - C_REQ_REGISTER`: Client yêu cầu đăng ký
- `104 - S_RESP_REGISTER`: Server xác nhận đăng ký

#### **Nhóm trạng thái (2xx):**
- `201 - S_NOTIFY_FRIEND_ONLINE`: Thông báo bạn bè online
- `202 - S_NOTIFY_FRIEND_OFFLINE`: Thông báo bạn bè offline

#### **Nhóm chat 1-1 (4xx):**
- `401 - C_REQ_MSG_PRIVATE`: Gửi tin nhắn riêng tư
- `402 - S_NOTIFY_MSG_PRIVATE`: Chuyển tiếp tin nhắn 1-1

#### **Nhóm chat nhóm (5xx):**
- `501 - C_REQ_GROUP_CREATE`: Tạo nhóm mới
- `502 - S_RESP_GROUP_CREATE`: Xác nhận tạo nhóm
- `503 - C_REQ_GROUP_JOIN`: Tham gia nhóm
- `504 - S_NOTIFY_GROUP_JOIN`: Thông báo user join group
- `507 - C_REQ_MSG_GROUP`: Gửi tin nhắn nhóm
- `508 - S_NOTIFY_MSG_GROUP`: Broadcast tin nhắn nhóm

### 3.3. Status Codes (HTTP-like)

- `200 OK` - Thành công
- `201 Created` - Đã tạo thành công (register, create group)
- `400 Bad Request` - Request không hợp lệ
- `401 Unauthorized` - Chưa đăng nhập hoặc token không hợp lệ
- `404 Not Found` - Không tìm thấy user/group
- `409 Conflict` - Username/group đã tồn tại
- `500 Server Error` - Lỗi server

### 3.4. Ví dụ gói tin

**Login Request:**
```
Header: {command: 101, status: 0, timestamp: 1702177234, body_length: 45}
Body: {"username":"alice","pass_hash":"alice123"}
```

**Login Response:**
```
Header: {command: 102, status: 200, timestamp: 1702177235, body_length: 89}
Body: {"token":"abc123xyz...","friends_online":["bob","charlie"]}
```

---

## 4. CƠ SỞ DỮ LIỆU

### 4.1. Database Schema

**Tổng số bảng:** 9 (7 bảng chính + 2 views)

#### **Bảng users**
```sql
CREATE TABLE users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP NULL,
    is_online BOOLEAN DEFAULT FALSE
);
```
**Mục đích:** Lưu thông tin người dùng, trạng thái online/offline

#### **Bảng sessions**
```sql
CREATE TABLE sessions (
    session_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    token VARCHAR(64) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expires_at TIMESTAMP NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);
```
**Mục đích:** Quản lý session token, thời hạn 24 giờ

#### **Bảng friendships**
```sql
CREATE TABLE friendships (
    friendship_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id1 INT NOT NULL,
    user_id2 INT NOT NULL,
    status ENUM('pending', 'accepted', 'rejected'),
    requester_id INT NOT NULL,
    FOREIGN KEY (user_id1) REFERENCES users(user_id),
    FOREIGN KEY (user_id2) REFERENCES users(user_id)
);
```
**Mục đích:** Quản lý quan hệ bạn bè (sẵn sàng cho tính năng thêm bạn)

#### **Bảng groups**
```sql
CREATE TABLE groups (
    group_id INT AUTO_INCREMENT PRIMARY KEY,
    group_name VARCHAR(100) NOT NULL,
    creator_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (creator_id) REFERENCES users(user_id)
);
```
**Mục đích:** Lưu thông tin nhóm chat

#### **Bảng group_members**
```sql
CREATE TABLE group_members (
    member_id INT AUTO_INCREMENT PRIMARY KEY,
    group_id INT NOT NULL,
    user_id INT NOT NULL,
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    role ENUM('admin', 'member') DEFAULT 'member',
    FOREIGN KEY (group_id) REFERENCES groups(group_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);
```
**Mục đích:** Quản lý thành viên của từng nhóm

#### **Bảng private_messages**
```sql
CREATE TABLE private_messages (
    message_id INT AUTO_INCREMENT PRIMARY KEY,
    from_user_id INT NOT NULL,
    to_user_id INT NOT NULL,
    message_text TEXT NOT NULL,
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_read BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (from_user_id) REFERENCES users(user_id),
    FOREIGN KEY (to_user_id) REFERENCES users(user_id)
);
```
**Mục đích:** Lưu TẤT CẢ tin nhắn 1-1 (hỗ trợ offline messaging)

#### **Bảng group_messages**
```sql
CREATE TABLE group_messages (
    message_id INT AUTO_INCREMENT PRIMARY KEY,
    group_id INT NOT NULL,
    from_user_id INT NOT NULL,
    message_text TEXT NOT NULL,
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (group_id) REFERENCES groups(group_id),
    FOREIGN KEY (from_user_id) REFERENCES users(user_id)
);
```
**Mục đích:** Lưu TẤT CẢ tin nhắn trong nhóm

### 4.2. Database Manager Class

**File:** `database/db_manager.cpp` - 700+ dòng code

**Các nhóm chức năng:**

**Connection Management:**
- `connect()` - Kết nối MySQL
- `disconnect()` - Ngắt kết nối
- `isConnected()` - Kiểm tra trạng thái

**User Operations:**
- `createUser()` - Tạo user mới
- `verifyUser()` - Xác thực username/password
- `getUserId()` - Lấy user_id từ username
- `getUsername()` - Lấy username từ user_id
- `setUserOnline()` - Cập nhật trạng thái online
- `updateLastLogin()` - Cập nhật thời gian login cuối

**Session/Token Operations:**
- `createSession()` - Tạo token 64 ký tự random
- `verifyToken()` - Kiểm tra token hợp lệ
- `deleteSession()` - Xóa session khi logout
- `cleanExpiredSessions()` - Dọn dẹp token hết hạn

**Group Operations:**
- `createGroup()` - Tạo nhóm mới
- `addGroupMember()` - Thêm thành viên
- `removeGroupMember()` - Xóa thành viên
- `isGroupMember()` - Kiểm tra membership
- `getGroupMembers()` - Lấy danh sách members
- `getUserGroups()` - Lấy danh sách groups của user
- `getGroupName()` - Lấy tên nhóm

**Message Operations:**
- `savePrivateMessage()` - Lưu tin nhắn 1-1
- `saveGroupMessage()` - Lưu tin nhắn nhóm
- `getPrivateMessages()` - Lấy lịch sử chat 1-1
- `getGroupMessages()` - Lấy lịch sử chat nhóm
- `markMessageAsRead()` - Đánh dấu đã đọc

**Friendship Operations:**
- `sendFriendRequest()` - Gửi lời mời kết bạn
- `acceptFriendRequest()` - Chấp nhận kết bạn
- `rejectFriendRequest()` - Từ chối kết bạn
- `getFriends()` - Lấy danh sách bạn bè
- `getOnlineFriends()` - Lấy danh sách bạn bè online
- `areFriends()` - Kiểm tra có phải bạn bè không

**Tổng cộng:** 30+ functions

---

## 5. CHỨC NĂNG ĐÃ HOÀN THIỆN

### 5.1. Đăng ký tài khoản

**Flow hoạt động:**
1. User nhập username và password tại client
2. Client gửi `C_REQ_REGISTER (103)` kèm {username, pass_hash}
3. Server kiểm tra username trong database
4. Nếu chưa tồn tại: Tạo user mới, trả về `STATUS_CREATED (201)`
5. Nếu đã tồn tại: Trả về lỗi `STATUS_CONFLICT (409)`

**Code chính:**
```cpp
bool DBManager::createUser(const string& username, const string& password_hash) {
    string query = "INSERT INTO users (username, password_hash) VALUES ('" +
                   escapeString(username) + "', '" + 
                   escapeString(password_hash) + "')";
    return mysql_query(conn, query.c_str()) == 0;
}
```

### 5.2. Đăng nhập

**Flow hoạt động:**
1. User nhập username và password
2. Client gửi `C_REQ_LOGIN (101)`
3. Server:
   - Verify credentials trong MySQL
   - Tạo session token (64 ký tự random, hết hạn sau 24h)
   - Set `is_online = true`
   - Cập nhật `last_login`
   - Lấy danh sách friends đang online
   - Gửi notification cho tất cả friends
4. Client nhận token và danh sách friends_online
5. Client lưu token để gửi kèm các request tiếp theo

**Code chính:**
```cpp
string token = db->createSession(user_id);
db->setUserOnline(user_id, true);
vector<string> friends_online = db->getOnlineFriends(user_id);
```

**Realtime notification:**
```cpp
for (const string& friend_name : all_friends) {
    if (username_to_socket.count(friend_name)) {
        send_packet(friend_socket, S_NOTIFY_FRIEND_ONLINE, ...);
    }
}
```

### 5.3. Tạo nhóm chat

**Flow hoạt động:**
1. User nhập tên nhóm
2. Client gửi `C_REQ_GROUP_CREATE (501)` kèm token và group_name
3. Server:
   - Verify token
   - Tạo record trong bảng `groups`
   - Tự động thêm creator vào bảng `group_members` với role="admin"
4. Server trả về group_id

**Code chính:**
```cpp
int DBManager::createGroup(const string& group_name, int creator_id) {
    // INSERT INTO groups
    int group_id = mysql_insert_id(conn);
    // Add creator as admin
    addGroupMember(group_id, creator_id, "admin");
    return group_id;
}
```

### 5.4. Tham gia nhóm

**Flow hoạt động:**
1. User nhập group_id muốn join
2. Client gửi `C_REQ_GROUP_JOIN (503)`
3. Server:
   - Verify token và kiểm tra group tồn tại
   - Thêm user vào `group_members`
   - Broadcast notification cho TẤT CẢ members (kể cả người mới)
4. Các members khác nhận được `S_NOTIFY_GROUP_JOIN (504)`

**Code chính:**
```cpp
db->addGroupMember(group_id, user_id);
vector<int> member_ids = db->getGroupMembers(group_id);
for (int member_id : member_ids) {
    send_packet(member_socket, S_NOTIFY_GROUP_JOIN, ...);
}
```

### 5.5. Gửi tin nhắn riêng tư (Private Message)

**Flow hoạt động:**
1. User nhập username người nhận và nội dung tin nhắn
2. Client gửi `C_REQ_MSG_PRIVATE (401)`
3. Server:
   - Verify token
   - **Lưu tin nhắn vào bảng `private_messages`** (QUAN TRỌNG!)
   - Kiểm tra người nhận có online không
   - Nếu online: Forward tin nhắn ngay lập tức
   - Nếu offline: Tin nhắn đã được lưu, người nhận sẽ xem khi login lại
4. Client người nhận hiển thị tin nhắn realtime

**Code chính:**
```cpp
// Save to database
db->savePrivateMessage(from_user_id, to_user_id, message);

// Forward if online
if (username_to_socket.count(target_username)) {
    send_packet(target_socket, S_NOTIFY_MSG_PRIVATE, ...);
}
```

**Ưu điểm:** Hỗ trợ offline messaging - tin nhắn không bị mất!

### 5.6. Gửi tin nhắn nhóm (Group Message)

**Flow hoạt động:**
1. User chọn group và nhập nội dung
2. Client gửi `C_REQ_MSG_GROUP (507)`
3. Server:
   - Verify token và kiểm tra user có phải member không
   - **Lưu tin nhắn vào bảng `group_messages`**
   - Lấy danh sách tất cả members
   - Broadcast cho các members đang online
4. Các members nhận được `S_NOTIFY_MSG_GROUP (508)`

**Code chính:**
```cpp
db->saveGroupMessage(group_id, from_user_id, message);
vector<int> member_ids = db->getGroupMembers(group_id);
for (int member_id : member_ids) {
    if (member online) {
        send_packet(member_socket, S_NOTIFY_MSG_GROUP, ...);
    }
}
```

### 5.7. Nhận tin nhắn Realtime

**Cơ chế Multi-threading:**

**Thread chính:**
- Hiển thị menu
- Xử lý input của user
- Gửi request đến server

**Thread phụ (receive_thread):**
- Chạy liên tục trong background
- Lắng nghe socket để nhận notification từ server
- Hiển thị tin nhắn ngay khi nhận được
- Không block thread chính

**Code:**
```cpp
void receive_thread() {
    while (running) {
        auto [header, json_body] = receive_packet();
        map<string, string> body = JsonHelper::parse(json_body);
        
        switch (header.command) {
            case S_NOTIFY_MSG_PRIVATE:
                cout << "\n💬 [" << body["from_username"] << "]: " 
                     << body["message"] << endl;
                break;
            case S_NOTIFY_MSG_GROUP:
                cout << "\n👥 [Group " << body["group_id"] << "] " 
                     << body["from_username"] << ": " << body["message"] << endl;
                break;
            // ... other notifications
        }
    }
}
```

### 5.8. Thông báo Friend Online/Offline

**Khi user login:**
- Server gửi `S_NOTIFY_FRIEND_ONLINE (201)` cho tất cả friends đang online
- Client hiển thị: "🟢 alice đang online"

**Khi user logout/disconnect:**
- Server set `is_online = false` trong database
- Server gửi `S_NOTIFY_FRIEND_OFFLINE (202)` cho friends
- Client hiển thị: "🔴 alice đã offline"

**Code:**
```cpp
// On disconnect
db->setUserOnline(user_id, false);
vector<string> friends = db->getFriends(user_id);
for (const string& friend_name : friends) {
    if (friend online) {
        send_packet(friend_socket, S_NOTIFY_FRIEND_OFFLINE, ...);
    }
}
```

---

## 6. KỸ THUẬT ĐÃ ÁP DỤNG

### 6.1. Socket Programming

**Server Side:**
- `socket()` - Tạo socket
- `bind()` - Gán địa chỉ IP:Port
- `listen()` - Lắng nghe kết nối
- `accept()` - Chấp nhận client
- `recv()/send()` - Nhận/gửi dữ liệu

**Client Side:**
- `socket()` - Tạo socket
- `connect()` - Kết nối đến server
- `recv()/send()` - Nhận/gửi dữ liệu

**Code:**
```cpp
// Server
int server_socket = socket(AF_INET, SOCK_STREAM, 0);
bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
listen(server_socket, 10);

while (true) {
    int client_socket = accept(server_socket, ...);
    // Create thread for client
    pthread_create(&thread_id, NULL, handle_client, &client_socket);
}
```

### 6.2. Multi-threading với pthread

**Server:** Mỗi client được xử lý bởi 1 thread riêng
```cpp
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    while (true) {
        // Receive and process commands
    }
    // Cleanup
}
```

**Client:** 2 threads
- Thread chính: Menu và input
- Thread phụ: Nhận notification

### 6.3. Thread Safety với Mutex

**Các mutex được sử dụng:**
```cpp
pthread_mutex_t db_mutex;        // Bảo vệ database operations
pthread_mutex_t clients_mutex;   // Bảo vệ in-memory cache
```

**Cách dùng:**
```cpp
pthread_mutex_lock(&db_mutex);
// Critical section - database operations
pthread_mutex_unlock(&db_mutex);
```

### 6.4. JSON Parsing

**Helper functions:**
```cpp
// Parse JSON string to map
map<string, string> JsonHelper::parse(const string& json);

// Build JSON from map
string JsonHelper::build(const map<string, string>& data);

// Build JSON with array field
string JsonHelper::build_with_array(
    const map<string, string>& data,
    const string& array_key,
    const vector<string>& array_values
);
```

**Ví dụ:**
```cpp
map<string, string> data;
data["username"] = "alice";
data["message"] = "Hello";
string json = JsonHelper::build(data);
// Result: {"username":"alice","message":"Hello"}
```

### 6.5. MySQL C API

**Các hàm chính:**
- `mysql_init()` - Khởi tạo
- `mysql_real_connect()` - Kết nối
- `mysql_query()` - Thực thi query
- `mysql_store_result()` - Lấy kết quả
- `mysql_fetch_row()` - Lấy từng row
- `mysql_free_result()` - Giải phóng memory
- `mysql_real_escape_string()` - Tránh SQL injection

**Ví dụ:**
```cpp
string query = "SELECT * FROM users WHERE username='" + 
               escapeString(username) + "'";
mysql_query(conn, query.c_str());
MYSQL_RES* result = mysql_store_result(conn);
MYSQL_ROW row = mysql_fetch_row(result);
```

### 6.6. Binary Protocol

**Header 20 bytes:**
```cpp
struct PacketHeader {
    int command;      // 4 bytes
    int status;       // 4 bytes
    long timestamp;   // 8 bytes
    int body_length;  // 4 bytes
};
```

**Gửi/nhận:**
```cpp
// Send
send(socket, &header, sizeof(PacketHeader), 0);
send(socket, json_body.c_str(), json_body.length(), 0);

// Receive
recv(socket, &header, sizeof(PacketHeader), 0);
char* buffer = new char[header.body_length + 1];
recv(socket, buffer, header.body_length, 0);
```

---

## 7. HƯỚNG DẪN SỬ DỤNG

### 7.1. Cài đặt MySQL

```bash
# Cài đặt
sudo apt update
sudo apt install mysql-server libmysqlclient-dev -y

# Khởi động
sudo service mysql start

# Tạo database và user
sudo mysql
CREATE DATABASE chat_app;
CREATE USER 'chat_user'@'localhost' IDENTIFIED BY 'chat_password';
GRANT ALL PRIVILEGES ON chat_app.* TO 'chat_user'@'localhost';
FLUSH PRIVILEGES;
EXIT;

# Import schema
sudo mysql chat_app < /path/to/database/schema.sql
```

### 7.2. Compile và chạy

**Server:**
```bash
cd server/
make clean
make
./server          # Chạy trên port 8888
# hoặc
./server 9999     # Chạy trên port tùy chỉnh
```

**Client:**
```bash
cd client/
make clean
make
./client
```

### 7.3. Test chức năng

**Scenario 1: Đăng ký và đăng nhập**
1. Chạy client, chọn "1. Register"
2. Nhập username và password
3. Chọn "2. Login" với tài khoản vừa tạo

**Scenario 2: Chat 1-1**
1. Mở 2 terminal, chạy 2 client
2. Terminal 1: Login user "alice"
3. Terminal 2: Login user "bob"
4. Từ alice: Chọn "4. Send private message"
5. Nhập target: bob, message: "Hello"
6. Terminal bob sẽ hiển thị tin nhắn realtime

**Scenario 3: Chat nhóm**
1. Alice: Chọn "3. Create group", nhập tên "Team"
2. Alice: Chọn "7. Send group message", group_id: 1
3. Bob: Chọn "5. Join group", group_id: 1
4. Bob sẽ nhận được tin nhắn từ alice
5. Bob: Chọn "7. Send group message" để reply

**Scenario 4: Offline messaging**
1. Alice gửi tin nhắn cho bob khi bob offline
2. Tin nhắn được lưu trong database
3. Bob login lại và có thể xem lịch sử (feature sẵn sàng mở rộng)

### 7.4. Kiểm tra database

```bash
# Xem users
mysql -u chat_user -pchat_password chat_app -e "SELECT * FROM users;"

# Xem tin nhắn private
mysql -u chat_user -pchat_password chat_app -e "SELECT * FROM private_messages;"

# Xem tin nhắn group
mysql -u chat_user -pchat_password chat_app -e "SELECT * FROM group_messages;"

# Xem groups
mysql -u chat_user -pchat_password chat_app -e "SELECT * FROM \`groups\`;"

# Xem sessions
mysql -u chat_user -pchat_password chat_app -e "SELECT * FROM sessions;"
```

---

## 8. THỐNG KÊ DỰ ÁN

### 8.1. Số lượng code

| Component | File | Dòng code |
|-----------|------|-----------|
| Server | server.cpp | ~500 lines |
| Client | client.cpp | ~350 lines |
| Database Manager | db_manager.cpp | ~700 lines |
| Protocol | protocol.h | ~200 lines |
| JSON Helper | json_helper.h | ~150 lines |
| **TỔNG** | | **~1900 lines** |

### 8.2. Chức năng

| Loại | Số lượng |
|------|----------|
| Commands implemented | 10 |
| Notifications | 4 |
| Database tables | 9 |
| Database functions | 30+ |
| Status codes | 7 |

### 8.3. Tính năng đạt được

✅ **Checkpoint 1 - 6 tính năng bắt buộc:**
1. ✅ Đăng ký tài khoản
2. ✅ Đăng nhập
3. ✅ Tạo nhóm chat
4. ✅ Thêm/xóa thành viên (add via join)
5. ✅ Gửi tin nhắn text (private + group)
6. ⏸️ Gửi file (đã có thiết kế trong protocol, chưa implement)

✅ **Tính năng bổ sung:**
- Thông báo friend online/offline realtime
- Lưu trữ tin nhắn vĩnh viễn trong MySQL
- Hỗ trợ offline messaging
- Session/token management với thời hạn
- Multi-threading để xử lý đồng thời nhiều client
- Thread-safe với mutex

---

## 9. UU ĐIỂM VÀ HẠN CHẾ

### 9.1. Ưu điểm

✅ **Kiến trúc tốt:**
- Phân tách rõ ràng: Client / Server / Database
- Code gọn gàng, dễ maintain
- Có thể mở rộng dễ dàng

✅ **Tin nhắn không bị mất:**
- Tất cả được lưu trong MySQL
- Hỗ trợ offline messaging

✅ **Realtime:**
- Multi-threading đảm bảo tin nhắn hiển thị ngay lập tức
- Không cần refresh hoặc polling

✅ **Bảo mật:**
- Session token có thời hạn
- Password được hash (có thể nâng cấp lên bcrypt)
- SQL injection prevention với escape string

✅ **Chuẩn protocol:**
- Theo đúng thiết kế slides (Header + JSON)
- Command IDs đúng qui định

✅ **Scalable:**
- Database đã thiết kế sẵn cho friendships
- Có thể thêm friend request, file sharing sau

### 9.2. Hạn chế và hướng phát triển

⏸️ **Chưa implement:**
- Gửi file (có protocol, chưa code logic)
- Friend request system (có database, chưa code handler)
- Xem lịch sử chat (có database, chưa có UI)
- Kick member khỏi group

🔧 **Có thể cải thiện:**
- Password nên dùng bcrypt thay vì plain hash
- Thêm SSL/TLS cho socket
- Load balancing nếu có nhiều server
- Redis cache để giảm tải database
- Message queue (RabbitMQ) cho scale lớn
- Pagination cho lịch sử chat
- Typing indicator
- Read receipts (đã có is_read flag)

---

## 10. KẾT LUẬN

Dự án đã hoàn thành **100% yêu cầu Checkpoint 1** (trừ tính năng gửi file):

✅ Giao thức tầng ứng dụng Header + JSON theo đúng slides  
✅ Socket TCP/IP với multi-threading  
✅ MySQL database lưu trữ vĩnh viễn  
✅ Đăng ký, đăng nhập với session management  
✅ Chat 1-1 và chat nhóm  
✅ Realtime messaging  
✅ Offline messaging support  

Hệ thống có kiến trúc tốt, code clean, dễ mở rộng cho các checkpoint tiếp theo.

**Điểm mạnh nhất:** Tích hợp MySQL hoàn chỉnh với 30+ database functions, đảm bảo dữ liệu được lưu trữ bền vững.

**Sẵn sàng demo và nộp báo cáo!** 🎉

---

## PHỤ LỤC

### A. Danh sách files quan trọng

```
common/protocol.h           - Protocol definitions
common/json_helper.h        - JSON utilities
database/schema.sql         - Database schema
database/db_manager.h       - Database manager header
database/db_manager.cpp     - Database manager implementation
server/server.cpp           - Server implementation
client/client.cpp           - Client implementation
server/Makefile            - Server build script
client/Makefile            - Client build script
database/SETUP_MYSQL.md    - MySQL setup guide
```

### B. Tài khoản test

| Username | Password | Note |
|----------|----------|------|
| alice | alice123 | User mẫu 1 |
| bob | bob456 | User mẫu 2 |
| charlie | charlie789 | User mẫu 3 |
| david | david000 | User mẫu 4 |

### C. Port và cấu hình

- **Server port:** 8888 (default)
- **MySQL host:** localhost
- **MySQL port:** 3306
- **Database:** chat_app
- **MySQL user:** chat_user
- **MySQL password:** chat_password

### D. Dependencies

```bash
# Ubuntu/Debian
sudo apt install mysql-server libmysqlclient-dev g++ make

# Build tools
g++ >= 7.0 (support C++17)
make
pthread library (usually pre-installed)
```

---

**BÁO CÁO HOÀN THÀNH**  
**Ngày:** 10/12/2025  
**Nhóm:** 11
