/*
 * GIAO THỨC TẦNG ỨNG DỤNG - CHECKPOINT 1
 * Nhóm 11 - Xây dựng ứng dụng Chat P2P
 * 
 * Kiến trúc: Client-Server
 * Cấu trúc gói tin: Header (20 bytes cố định) + Body (JSON string)
 * 
 * Header Format:
 *   - command (4 bytes): ID lệnh
 *   - status (4 bytes): Mã trạng thái HTTP-like
 *   - timestamp (8 bytes): Thời gian gửi (Unix timestamp)
 *   - body_length (4 bytes): Độ dài phần Body
 * 
 * Body Format: JSON string
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstring>
#include <ctime>

// ===== NHÓM XÁC THỰC (1xx) =====
#define C_REQ_LOGIN         101   // Client yêu cầu đăng nhập
#define S_RESP_LOGIN        102   // Server phản hồi đăng nhập
#define C_REQ_REGISTER      103   // Client yêu cầu đăng ký
#define S_RESP_REGISTER     104   // Server xác nhận đăng ký
#define C_REQ_CHANGE_PASS   105   // Client yêu cầu đổi mật khẩu
#define S_RESP_CHANGE_PASS  106   // Server phản hồi đổi mật khẩu

// ===== NHÓM TRẠNG THÁI (2xx) =====
#define S_NOTIFY_FRIEND_ONLINE  201   // Server báo bạn bè online
#define S_NOTIFY_FRIEND_OFFLINE 202   // Server báo bạn bè offline

// ===== NHÓM QUẢN LÝ BẠN BÈ (3xx) =====
#define C_REQ_FRIEND_ADD        301   // Client gửi lời mời kết bạn (FC05)
#define S_NOTIFY_FRIEND_REQ     302   // Server báo u5 biết u1 muốn kết bạn
#define C_RESP_FRIEND_REQ       303   // Client (u5) phản hồi lời mời
#define S_NOTIFY_FRIEND_ACCEPT  304   // Server báo u1 biết u5 đã đồng ý
#define C_REQ_FRIEND_LIST       305   // Client yêu cầu danh sách bạn bè
#define S_RESP_FRIEND_LIST      306   // Server trả về danh sách bạn bè
#define C_REQ_PENDING_REQUESTS  307   // Client yêu cầu danh sách lời mời chờ
#define S_RESP_PENDING_REQUESTS 308   // Server trả về danh sách lời mời chờ
#define C_REQ_UNFRIEND          309   // Client yêu cầu hủy kết bạn
#define S_RESP_UNFRIEND         310   // Server phản hồi hủy kết bạn

// ===== NHÓM CHAT 1-1 (4xx) =====
#define C_REQ_MSG_PRIVATE   401   // Client gửi tin nhắn 1-1 (FC03)
#define S_NOTIFY_MSG_PRIVATE 402  // Server chuyển tiếp tin nhắn 1-1

// ===== NHÓM CHAT NHÓM (5xx) =====
#define C_REQ_GROUP_CREATE  501   // Client yêu cầu tạo nhóm mới
#define S_RESP_GROUP_CREATE 502   // Server xác nhận tạo nhóm
#define C_REQ_GROUP_JOIN    503   // Client yêu cầu tham gia nhóm (FC07)
#define S_NOTIFY_GROUP_JOIN 504   // Server báo các thành viên (và người mới) đã vào nhóm
#define C_REQ_GROUP_LEAVE   505   // Client yêu cầu rời nhóm (FC07)
#define S_NOTIFY_GROUP_LEAVE 506  // Server báo các thành viên là u1 đã rời nhóm
#define C_REQ_MSG_GROUP     507   // Client gửi tin nhắn vào nhóm (FC08)
#define S_NOTIFY_MSG_GROUP  508   // Server broadcast tin nhắn cho các thành viên
#define C_REQ_GROUP_LIST    509   // Client yêu cầu danh sách nhóm đã tham gia
#define S_RESP_GROUP_LIST   510   // Server trả về danh sách nhóm
#define C_REQ_GROUP_MEMBERS 511   // Client yêu cầu danh sách thành viên nhóm
#define S_RESP_GROUP_MEMBERS 512  // Server trả về danh sách thành viên nhóm

// ===== NHÓM LỊCH SỬ CHAT (7xx) =====
#define C_REQ_CHAT_HISTORY_PRIVATE  701   // Client yêu cầu lịch sử chat 1-1
#define S_RESP_CHAT_HISTORY_PRIVATE 702   // Server trả về lịch sử chat 1-1
#define C_REQ_CHAT_HISTORY_GROUP    703   // Client yêu cầu lịch sử chat nhóm
#define S_RESP_CHAT_HISTORY_GROUP   704   // Server trả về lịch sử chat nhóm
#define C_REQ_MARK_MESSAGES_READ    705   // Client đánh dấu đã đọc tin nhắn
#define S_NOTIFY_MESSAGES_READ      706   // Server báo người gửi tin nhắn đã được đọc

// ===== NHÓM CHIA SẺ FILE (6xx) =====
#define C_REQ_FILE_UPLOAD   601   // Client "xin phép" được tải file lên
#define S_RESP_FILE_OK      602   // Server đồng ý, trả về file_id để Client bắt đầu gửi
#define C_DATA_FILE_END     604   // Client báo đã gửi xong chunk cuối cùng
#define S_NOTIFY_FILE_NEW   605   // Server báo các thành viên nhóm có file mới
#define C_REQ_FILE_DOWNLOAD 606   // Client yêu cầu được tải file
#define S_RESP_FILE_START   607   // Server đồng ý, báo Client chuẩn bị nhận file
#define S_DATA_FILE_END     609   // Server báo đã gửi xong chunk cuối cùng

// ===== NHÓM GÓI TIN CÓ PHẦN BODY LÀ DỮ LIỆU NHỊ PHÂN (16xx) =====
#define C_DATA_FILE_CHUNK   1603  // Client gửi một khối (chunk) của file
#define S_DATA_FILE_CHUNK   1608  // Server gửi một khối (chunk) của file

// ===== STATUS CODES (HTTP-like) =====
#define STATUS_OK           200   // Thành công
#define STATUS_CREATED      201   // Đã tạo thành công
#define STATUS_BAD_REQUEST  400   // Request không hợp lệ
#define STATUS_UNAUTHORIZED 401   // Chưa đăng nhập
#define STATUS_FORBIDDEN    403   // Không có quyền
#define STATUS_NOT_FOUND    404   // Không tìm thấy
#define STATUS_CONFLICT     409   // Conflict (username/group đã tồn tại)
#define STATUS_SERVER_ERROR 500   // Lỗi server

// ===== PACKET HEADER (20 bytes cố định) =====
struct PacketHeader {
    int command;          // 4 bytes - ID lệnh
    int status;           // 4 bytes - Mã trạng thái
    long timestamp;       // 8 bytes - Thời gian (Unix timestamp)
    int body_length;      // 4 bytes - Độ dài Body
    
    PacketHeader() : command(0), status(0), timestamp(0), body_length(0) {}
    
    PacketHeader(int cmd, int stat = STATUS_OK) 
        : command(cmd), status(stat), timestamp(time(NULL)), body_length(0) {}
};

// ===== CONSTANTS =====
#define MAX_BODY_SIZE 65535   // 64KB max cho JSON body
#define FILE_CHUNK_SIZE 4096  // 4KB per chunk cho file transfer

// ===== JSON BODY EXAMPLES =====
/*
C_REQ_LOGIN (101):
{
    "username": "u1",
    "pass_hash": "..."
}

S_RESP_LOGIN (102):
(200 OK) {
    "token": "...",
    "friends_online": [...]
}

C_REQ_REGISTER (103):
{
    "username": "u1",
    "pass_hash": "..."
}

S_RESP_REGISTER (104):
(201 OK) {
    "message": "Register OK"
}

S_NOTIFY_FRIEND_ONLINE (201):
{
    "username": "u4"
}

S_NOTIFY_FRIEND_OFFLINE (202):
{
    "username": "u2"
}

C_REQ_FRIEND_ADD (301):
{
    "token": "...",
    "target_username": "u5"
}

S_NOTIFY_FRIEND_REQ (302):
{
    "from_username": "u1"
}

C_RESP_FRIEND_REQ (303):
{
    "token": "...",
    "from_username": "u1",
    "accept": true
}

S_NOTIFY_FRIEND_ACCEPT (304):
{
    "username": "u5"
}

C_REQ_MSG_PRIVATE (401):
{
    "token": "...",
    "target_username": "u2",
    "message": "Hi"
}

S_NOTIFY_MSG_PRIVATE (402):
{
    "from_username": "u1",
    "message": "Hi"
}

C_REQ_GROUP_CREATE (501):
{
    "token": "...",
    "group_name": "Class 101"
}

S_RESP_GROUP_CREATE (502):
(201 OK) {
    "group_id": "1",
    "group_name": "..."
}

C_REQ_GROUP_JOIN (503):
{
    "token": "...",
    "group_id": "1"
}

S_NOTIFY_GROUP_JOIN (504):
{
    "username": "u1",
    "group_id": "1"
}

C_REQ_GROUP_LEAVE (505):
{
    "token": "...",
    "group_id": "1"
}

S_NOTIFY_GROUP_LEAVE (506):
{
    "username": "u1",
    "group_id": "1"
}

C_REQ_MSG_GROUP (507):
{
    "token": "...",
    "group_id": "1",
    "message": "Hello all"
}

S_NOTIFY_MSG_GROUP (508):
{
    "from_username": "u1",
    "group_id": "1",
    "message": "..."
}

C_REQ_FILE_UPLOAD (601):
{
    "token": "...",
    "group_id": "1",
    "filename": "a.zip",
    "filesize": 10240
}

S_RESP_FILE_OK (602):
(200 OK) {
    "file_id": 123,
    "message": "Ready"
}

C_DATA_FILE_END (604):
{
    "token": "...",
    "file_id": 123
}

S_NOTIFY_FILE_NEW (605):
{
    "from_user": "u1",
    "group_id": "1",
    "filename": "a.zip",
    "file_id": 123
}

C_REQ_FILE_DOWNLOAD (606):
{
    "token": "...",
    "file_id": 123
}

S_RESP_FILE_START (607):
(200 OK) {
    "filename": "a.zip",
    "filesize": 10240
}

S_DATA_FILE_END (609):
{
    "file_id": 123,
    "message": "Download complete"
}

C_DATA_FILE_CHUNK (1603):
[Dữ liệu nhị phân (vd: 4KB)]

S_DATA_FILE_CHUNK (1608):
[Dữ liệu nhị phân (vd: 4KB)]
*/

#endif // PROTOCOL_H
