# ỨNG DỤNG CHAT P2P - NHÓM 11

## 🎯 Checkpoint 1 - Các tính năng đã hoàn thành

### ✅ Tính năng cơ bản:
1. **Đăng ký tài khoản** - User có thể tạo tài khoản mới
2. **Đăng nhập** - Xác thực username/password
3. **Tạo nhóm chat** - Tạo nhóm mới để chat
4. **Vào/rời nhóm** - Join/leave group với thông báo
5. **Gửi tin nhắn nhóm** - Broadcast đến tất cả members
6. **Gửi tin nhắn riêng** - Chat 1-1 giữa 2 users
7. **Xem danh sách nhóm** - List tất cả groups

### 📊 Kiến trúc:
- **Model**: Client-Server
- **Threading**: pthread cho mỗi client
- **Synchronization**: 3 mutexes (db, clients, groups)
- **Protocol**: Binary message-based

## 🚀 Cách chạy

### Build:
```bash
# Server
cd server
make

# Client
cd client
make
```

### Run:
```bash
# Terminal 1: Server
cd server
./server

# Terminal 2+: Clients
cd client
./client
```

### Test accounts:
- admin/password
- alice/password
- bob/password
- charlie/password

## 📋 Menu chức năng

```
1. Tin nhắn riêng    - Chat 1-1
2. Chat nhóm         - Broadcast trong group
3. Tạo nhóm          - Create new group
4. Vào nhóm          - Join existing group
5. Xem danh sách nhóm - List all groups
0. Thoát
```

## 🎓 Demo scenario

### Scenario 1: Chat nhóm
```
Alice: Tạo nhóm "project"
Bob: Vào nhóm "project"
Charlie: Vào nhóm "project"
→ Alice chat: "Hello team!"
→ Bob và Charlie nhận tin nhắn
```

### Scenario 2: Chat riêng
```
Alice: Chọn tin nhắn riêng → bob
Alice: "Bạn có rảnh không?"
→ Bob nhận tin nhắn private
```

## 📁 Cấu trúc

```
chat_app/
├── server/
│   ├── server.cpp
│   ├── server (binary)
│   └── Makefile
├── client/
│   ├── client.cpp
│   ├── client (binary)
│   └── Makefile
├── common/
│   └── protocol.h
├── database/
│   └── users.txt
└── README.md
```

## 🔧 Kỹ thuật sử dụng

1. **Socket TCP/IP** - Reliable connection
2. **pthread** - Multi-threading
3. **Mutex** - Thread synchronization
4. **STL map/set** - Data structures
5. **Binary protocol** - Efficient communication

## 📝 Ghi chú

- Port: 8888
- Max content: 1000 bytes
- Thread-safe operations
- Auto cleanup on disconnect

---

**Nhóm 11** - Checkpoint 1 hoàn thành! ✅
