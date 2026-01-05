# Hướng dẫn cài đặt MySQL và thiết lập Database

## Bước 1: Cài đặt MySQL Server và Client

```bash
# Update package list
sudo apt update

# Cài MySQL server và client
sudo apt install mysql-server mysql-client -y

# Cài thư viện development để compile C++ với MySQL
sudo apt install libmysqlclient-dev -y
```

## Bước 2: Khởi động MySQL service

```bash
# Start MySQL service
sudo service mysql start

# Kiểm tra status
sudo service mysql status
```

## Bước 3: Thiết lập MySQL (lần đầu)

```bash
# Chạy script bảo mật (optional, có thể skip cho development)
sudo mysql_secure_installation
```

Hoặc đơn giản hơn:

```bash
# Login vào MySQL với root
sudo mysql

# Trong MySQL prompt, tạo user và database:
CREATE DATABASE chat_app;
CREATE USER 'chat_user'@'localhost' IDENTIFIED BY 'chat_password';
GRANT ALL PRIVILEGES ON chat_app.* TO 'chat_user'@'localhost';
FLUSH PRIVILEGES;
EXIT;
```

## Bước 4: Import schema vào database

```bash
# Dùng root user
sudo mysql chat_app < /mnt/e/chat_app/database/schema.sql

# Hoặc dùng user vừa tạo
mysql -u chat_user -p chat_app < /mnt/e/chat_app/database/schema.sql
# Nhập password: chat_password
```

## Bước 5: Verify database đã được tạo

```bash
mysql -u chat_user -p

# Trong MySQL prompt:
USE chat_app;
SHOW TABLES;
SELECT * FROM users;
EXIT;
```

## Bước 6: Update Makefile để compile với MySQL

File `server/Makefile` cần thêm flags:

```makefile
MYSQL_FLAGS = $(shell mysql_config --cflags --libs)

server: server.cpp
	g++ -std=c++17 -Wall -I../common -I../database server.cpp ../database/db_manager.cpp -o server -pthread $(MYSQL_FLAGS)
```

## Bước 7: Test connection

```bash
cd /mnt/e/chat_app/server
make clean
make
./server
```

## Troubleshooting

### Lỗi: Can't connect to MySQL server
```bash
# Kiểm tra MySQL có chạy không
sudo service mysql status

# Nếu stopped, start lại
sudo service mysql start
```

### Lỗi: Access denied for user
```bash
# Reset password hoặc tạo lại user
sudo mysql
DROP USER 'chat_user'@'localhost';
CREATE USER 'chat_user'@'localhost' IDENTIFIED BY 'chat_password';
GRANT ALL PRIVILEGES ON chat_app.* TO 'chat_user'@'localhost';
FLUSH PRIVILEGES;
```

### Lỗi: mysql_config: command not found
```bash
# Cài lại development package
sudo apt install libmysqlclient-dev
```

## Cấu hình kết nối trong code

File `server/server.cpp`, thêm ở đầu main():

```cpp
// Khởi tạo database connection
DBManager db("localhost", "chat_user", "chat_password", "chat_app", 3306);
if (!db.connect()) {
    cerr << "Cannot connect to database" << endl;
    return 1;
}
```

## Các lệnh MySQL hữu ích

```bash
# Login
mysql -u chat_user -p

# Xem database
SHOW DATABASES;

# Chọn database
USE chat_app;

# Xem tables
SHOW TABLES;

# Xem structure của table
DESCRIBE users;

# Query data
SELECT * FROM users;
SELECT * FROM groups;
SELECT * FROM private_messages;

# Delete data (careful!)
TRUNCATE TABLE private_messages;
TRUNCATE TABLE group_messages;

# Drop database (careful!)
DROP DATABASE chat_app;
```
