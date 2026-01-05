-- =============================================
-- CHAT APPLICATION DATABASE SCHEMA
-- Checkpoint 1 - MySQL Database
-- =============================================

CREATE DATABASE IF NOT EXISTS chat_app;
USE chat_app;

-- =============================================
-- TABLE: users
-- Lưu thông tin người dùng
-- =============================================
CREATE TABLE IF NOT EXISTS users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP NULL,
    is_online BOOLEAN DEFAULT FALSE,
    INDEX idx_username (username),
    INDEX idx_online (is_online)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- TABLE: friendships
-- Lưu quan hệ bạn bè giữa các users
-- =============================================
CREATE TABLE IF NOT EXISTS friendships (
    friendship_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id1 INT NOT NULL,
    user_id2 INT NOT NULL,
    status ENUM('pending', 'accepted', 'rejected') DEFAULT 'pending',
    requester_id INT NOT NULL, -- Người gửi lời mời
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id1) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (user_id2) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (requester_id) REFERENCES users(user_id) ON DELETE CASCADE,
    UNIQUE KEY unique_friendship (user_id1, user_id2),
    INDEX idx_user1 (user_id1),
    INDEX idx_user2 (user_id2),
    INDEX idx_status (status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- TABLE: groups
-- Lưu thông tin nhóm chat
-- =============================================
CREATE TABLE IF NOT EXISTS `groups` (
    group_id INT AUTO_INCREMENT PRIMARY KEY,
    group_name VARCHAR(100) NOT NULL,
    creator_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (creator_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_creator (creator_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- TABLE: group_members
-- Lưu thành viên của các nhóm
-- =============================================
CREATE TABLE IF NOT EXISTS group_members (
    member_id INT AUTO_INCREMENT PRIMARY KEY,
    group_id INT NOT NULL,
    user_id INT NOT NULL,
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    role ENUM('admin', 'member') DEFAULT 'member',
    FOREIGN KEY (group_id) REFERENCES `groups`(group_id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    UNIQUE KEY unique_member (group_id, user_id),
    INDEX idx_group (group_id),
    INDEX idx_user (user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- TABLE: private_messages
-- Lưu tin nhắn 1-1 giữa 2 users
-- =============================================
CREATE TABLE IF NOT EXISTS private_messages (
    message_id INT AUTO_INCREMENT PRIMARY KEY,
    from_user_id INT NOT NULL,
    to_user_id INT NOT NULL,
    message_text TEXT NOT NULL,
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_read BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (from_user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (to_user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_from (from_user_id),
    INDEX idx_to (to_user_id),
    INDEX idx_sent (sent_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- TABLE: group_messages
-- Lưu tin nhắn trong nhóm
-- =============================================
CREATE TABLE IF NOT EXISTS group_messages (
    message_id INT AUTO_INCREMENT PRIMARY KEY,
    group_id INT NOT NULL,
    from_user_id INT NOT NULL,
    message_text TEXT NOT NULL,
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (group_id) REFERENCES `groups`(group_id) ON DELETE CASCADE,
    FOREIGN KEY (from_user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_group (group_id),
    INDEX idx_from (from_user_id),
    INDEX idx_sent (sent_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- TABLE: sessions
-- Lưu session/token của users đang login
-- =============================================
CREATE TABLE IF NOT EXISTS sessions (
    session_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    token VARCHAR(64) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expires_at TIMESTAMP NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_token (token),
    INDEX idx_user (user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- =============================================
-- INSERT SAMPLE DATA
-- Tạo một vài users mẫu
-- =============================================
INSERT INTO users (username, password_hash) VALUES
('alice', 'alice123'),
('bob', 'bob456'),
('charlie', 'charlie789'),
('david', 'david000')
ON DUPLICATE KEY UPDATE username=username;

-- Tạo friendships mẫu
INSERT INTO friendships (user_id1, user_id2, status, requester_id) VALUES
(1, 2, 'accepted', 1),  -- alice & bob
(1, 3, 'accepted', 1),  -- alice & charlie
(2, 3, 'accepted', 2)   -- bob & charlie
ON DUPLICATE KEY UPDATE status=status;

-- =============================================
-- VIEWS để query dễ hơn
-- =============================================

-- View: Lấy danh sách bạn bè của user
CREATE OR REPLACE VIEW user_friends AS
SELECT 
    CASE 
        WHEN f.user_id1 = u1.user_id THEN u2.user_id
        ELSE u1.user_id
    END AS user_id,
    CASE 
        WHEN f.user_id1 = u1.user_id THEN u2.username
        ELSE u1.username
    END AS friend_username,
    f.status
FROM friendships f
JOIN users u1 ON f.user_id1 = u1.user_id
JOIN users u2 ON f.user_id2 = u2.user_id
WHERE f.status = 'accepted';

-- View: Lấy members của từng group
CREATE OR REPLACE VIEW group_members_view AS
SELECT 
    g.group_id,
    g.group_name,
    u.user_id,
    u.username,
    gm.role,
    gm.joined_at
FROM `groups` g
JOIN group_members gm ON g.group_id = gm.group_id
JOIN users u ON gm.user_id = u.user_id;
