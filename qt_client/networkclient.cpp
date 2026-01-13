#include "networkclient.h"
#include "chatwidget.h"
#include <QDataStream>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    
    connect(m_socket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &NetworkClient::onError);
}

NetworkClient::~NetworkClient()
{
    disconnectFromServer();
}

bool NetworkClient::connectToServer(const QString &host, int port)
{
    m_socket->connectToHost(host, port);
    return m_socket->waitForConnected(5000);
}

void NetworkClient::disconnectFromServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
}

bool NetworkClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkClient::onConnected()
{
    emit connected();
}

void NetworkClient::onDisconnected()
{
    emit disconnected();
}

void NetworkClient::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    emit connectionError(m_socket->errorString());
}

QString NetworkClient::buildJson(const QMap<QString, QString> &data)
{
    QString json = "{";
    bool first = true;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (!first) json += ",";
        json += "\"" + it.key() + "\":\"" + it.value() + "\"";
        first = false;
    }
    json += "}";
    return json;
}

QMap<QString, QString> NetworkClient::parseJson(const QString &json)
{
    QMap<QString, QString> result;
    QString s = json;
    s.remove('{').remove('}');
    
    // Simple JSON parser for key-value pairs
    int pos = 0;
    while (pos < s.length()) {
        // Find key
        int keyStart = s.indexOf('"', pos);
        if (keyStart == -1) break;
        int keyEnd = s.indexOf('"', keyStart + 1);
        if (keyEnd == -1) break;
        QString key = s.mid(keyStart + 1, keyEnd - keyStart - 1);
        
        // Find value
        int colonPos = s.indexOf(':', keyEnd);
        if (colonPos == -1) break;
        
        // Check if value is string or other type
        int valueStart = s.indexOf('"', colonPos);
        int nextComma = s.indexOf(',', colonPos);
        
        if (valueStart != -1 && (nextComma == -1 || valueStart < nextComma)) {
            int valueEnd = s.indexOf('"', valueStart + 1);
            if (valueEnd != -1) {
                result[key] = s.mid(valueStart + 1, valueEnd - valueStart - 1);
                pos = valueEnd + 1;
            } else {
                break;
            }
        } else {
            // Non-string value (number, boolean)
            int endPos = (nextComma != -1) ? nextComma : s.length();
            QString value = s.mid(colonPos + 1, endPos - colonPos - 1).trimmed();
            result[key] = value;
            pos = endPos + 1;
        }
    }
    return result;
}

void NetworkClient::sendPacket(int command, const QMap<QString, QString> &body)
{
    QString jsonStr = buildJson(body);
    QByteArray jsonBytes = jsonStr.toUtf8();
    
    PacketHeader header;
    header.command = command;
    header.status = STATUS_OK;
    header.timestamp = time(nullptr);
    header.body_length = jsonBytes.size();
    
    m_socket->write(reinterpret_cast<char*>(&header), sizeof(PacketHeader));
    m_socket->write(jsonBytes);
    m_socket->flush();
}

void NetworkClient::onReadyRead()
{
    m_buffer.append(m_socket->readAll());
    
    while (m_buffer.size() >= (int)sizeof(PacketHeader)) {
        PacketHeader header;
        memcpy(&header, m_buffer.data(), sizeof(PacketHeader));
        
        int totalSize = sizeof(PacketHeader) + header.body_length;
        if (m_buffer.size() < totalSize) {
            break; // Wait for more data
        }
        
        QByteArray body = m_buffer.mid(sizeof(PacketHeader), header.body_length);
        m_buffer.remove(0, totalSize);
        
        processPacket(header, body);
    }
}

void NetworkClient::processPacket(const PacketHeader &header, const QByteArray &body)
{
    QString jsonStr = QString::fromUtf8(body);
    QMap<QString, QString> data = parseJson(jsonStr);
    
    switch (header.command) {
        case S_RESP_LOGIN:
            if (header.status == STATUS_OK) {
                m_token = data.value("token");
                emit loginResponse(true, "Đăng nhập thành công", m_token);
            } else {
                emit loginResponse(false, data.value("message", "Đăng nhập thất bại"), "");
            }
            break;
            
        case S_RESP_REGISTER:
            emit registerResponse(header.status == STATUS_OK || header.status == STATUS_CREATED,
                                  data.value("message"));
            break;
            
        case S_RESP_CHANGE_PASS:
            emit changePasswordResponse(header.status == STATUS_OK,
                                        data.value("message"));
            break;
            
        case S_RESP_GROUP_CREATE:
            emit groupCreated(header.status == STATUS_OK || header.status == STATUS_CREATED,
                             data.value("message"), data.value("group_id"));
            break;
            
        case S_RESP_FILE_OK:
            if (header.status == STATUS_OK) {
                QString message = data.value("message");
                if (message.contains("download")) {
                    // File download response
                    QString fileName = data.value("file_name");
                    QString fileDataBase64 = data.value("file_data");
                    qint64 fileSize = data.value("file_size", "0").toLongLong();
                    
                    if (!fileDataBase64.isEmpty()) {
                        QByteArray fileData = QByteArray::fromBase64(fileDataBase64.toLatin1());
                        emit fileDownloadReceived(fileName, fileData, fileSize);
                        qDebug() << "File download received:" << fileName << "Size:" << fileSize;
                    }
                } else {
                    qDebug() << "File uploaded successfully:" << data.value("file_name");
                }
            } else {
                qDebug() << "File operation failed:" << data.value("message");
            }
            break;
            
        case S_RESP_DELETE_MESSAGE: {
            bool success = (header.status == STATUS_OK);
            int messageId = data.value("message_id", "-1").toInt();
            emit deleteMessageResponse(success, data.value("message"), messageId);
            break;
        }
        
        case S_NOTIFY_MESSAGE_DELETED: {
            int messageId = data.value("message_id", "-1").toInt();
            QString chatType = data.value("chat_type");
            QString groupId = data.value("group_id", "");
            emit messageDeleted(messageId, chatType, groupId);
            break;
        }
            
        case S_RESP_GROUP_LIST: {
            QStringList groups;
            // Parse groups array from JSON
            int start = jsonStr.indexOf('[');
            int end = jsonStr.lastIndexOf(']');
            if (start != -1 && end != -1) {
                QString listStr = jsonStr.mid(start + 1, end - start - 1);
                // Parse each group object
                int pos = 0;
                while ((pos = listStr.indexOf("{\"group_id\":", pos)) != -1) {
                    int idStart = listStr.indexOf('"', pos + 12) + 1;
                    int idEnd = listStr.indexOf('"', idStart);
                    QString groupId = listStr.mid(idStart, idEnd - idStart);
                    
                    int namePos = listStr.indexOf("\"group_name\":", idEnd);
                    int nameStart = listStr.indexOf('"', namePos + 13) + 1;
                    int nameEnd = listStr.indexOf('"', nameStart);
                    QString groupName = listStr.mid(nameStart, nameEnd - nameStart);
                    
                    groups.append(groupId + ":" + groupName);
                    pos = nameEnd + 1;
                }
            }
            emit groupListReceived(groups);
            break;
        }
        
        case S_RESP_ALL_GROUPS: {
            QList<QPair<QString, QString>> groups;
            // Parse groups array from JSON
            int start = jsonStr.indexOf('[');
            int end = jsonStr.lastIndexOf(']');
            if (start != -1 && end != -1) {
                QString listStr = jsonStr.mid(start + 1, end - start - 1);
                // Parse each group object
                int pos = 0;
                while ((pos = listStr.indexOf("{\"group_id\":", pos)) != -1) {
                    int idStart = listStr.indexOf('"', pos + 12) + 1;
                    int idEnd = listStr.indexOf('"', idStart);
                    QString groupId = listStr.mid(idStart, idEnd - idStart);
                    
                    int namePos = listStr.indexOf("\"group_name\":", idEnd);
                    int nameStart = listStr.indexOf('"', namePos + 13) + 1;
                    int nameEnd = listStr.indexOf('"', nameStart);
                    QString groupName = listStr.mid(nameStart, nameEnd - nameStart);
                    
                    int countPos = listStr.indexOf("\"member_count\":", nameEnd);
                    int countStart = listStr.indexOf('"', countPos + 15) + 1;
                    int countEnd = listStr.indexOf('"', countStart);
                    QString memberCount = listStr.mid(countStart, countEnd - countStart);
                    
                    QString displayText = groupName + " (" + memberCount + " members)";
                    groups.append(qMakePair(groupId, displayText));
                    pos = countEnd + 1;
                }
            }
            
            // Show dialog directly instead of using signals
            QDialog dialog;
            dialog.setWindowTitle("Chọn nhóm để tham gia");
            dialog.resize(400, 300);
            
            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            QListWidget *listWidget = new QListWidget(&dialog);
            
            for (const auto &group : groups) {
                listWidget->addItem(group.first + " - " + group.second);
            }
            
            layout->addWidget(listWidget);
            
            QHBoxLayout *buttonLayout = new QHBoxLayout;
            QPushButton *joinBtn = new QPushButton("Tham gia");
            QPushButton *cancelBtn = new QPushButton("Hủy");
            buttonLayout->addWidget(joinBtn);
            buttonLayout->addWidget(cancelBtn);
            layout->addLayout(buttonLayout);
            
            QObject::connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
            QObject::connect(joinBtn, &QPushButton::clicked, [this, &dialog, &listWidget, &groups]() {
                if (listWidget->currentRow() >= 0) {
                    QString groupId = groups[listWidget->currentRow()].first;
                    QMap<QString, QString> body;
                    body["token"] = m_token;
                    body["group_id"] = groupId;
                    sendPacket(C_REQ_GROUP_JOIN, body);
                    dialog.accept();
                }
            });
            
            dialog.exec();
            break;
        }
        
        case S_RESP_GROUP_MEMBERS: {
            QString groupId = data.value("group_id");
            QString groupName = data.value("group_name");
            QList<QPair<QString, bool>> members;
            
            // Parse members array from raw JSON string
            int membersStart = jsonStr.indexOf("\"members\":");
            if (membersStart != -1) {
                int arrStart = jsonStr.indexOf('[', membersStart);
                int arrEnd = jsonStr.indexOf(']', arrStart);
                if (arrStart != -1 && arrEnd != -1) {
                    QString membersStr = jsonStr.mid(arrStart, arrEnd - arrStart + 1);
                    int pos = 0;
                    while ((pos = membersStr.indexOf("{\"username\":", pos)) != -1) {
                        int nameStart = membersStr.indexOf('"', pos + 12) + 1;
                        int nameEnd = membersStr.indexOf('"', nameStart);
                        QString username = membersStr.mid(nameStart, nameEnd - nameStart);
                        
                        int onlinePos = membersStr.indexOf("\"online\":", nameEnd);
                        bool isOnline = membersStr.indexOf("true", onlinePos) == onlinePos + 9;
                        
                        members.append(qMakePair(username, isOnline));
                        pos = nameEnd + 1;
                    }
                }
            }
            emit groupMembersReceived(groupId, groupName, members);
            break;
        }
        
        case S_RESP_CHAT_HISTORY_PRIVATE: {
            QString targetUser = data.value("target_username");
            int totalCount = data.value("total_count", "0").toInt();
            int offset = data.value("offset", "0").toInt();
            QList<QMap<QString, QString>> messages;
            
            // Parse messages array
            int start = jsonStr.indexOf('[');
            int end = jsonStr.lastIndexOf(']');
            if (start != -1 && end != -1) {
                QString listStr = jsonStr.mid(start + 1, end - start - 1);
                int pos = 0;
                while ((pos = listStr.indexOf("{", pos)) != -1) {
                    QMap<QString, QString> msg;
                    
                    // Parse message_id (number, not string)
                    int idPos = listStr.indexOf("\"message_id\":", pos);
                    if (idPos != -1) {
                        int idStart = idPos + 13;
                        int idEnd = listStr.indexOf(',', idStart);
                        if (idEnd != -1) {
                            msg["message_id"] = listStr.mid(idStart, idEnd - idStart).trimmed();
                        }
                    }
                    
                    int fromPos = listStr.indexOf("\"from_username\":", pos);
                    if (fromPos != -1) {
                        int fromStart = listStr.indexOf('"', fromPos + 16) + 1;
                        int fromEnd = listStr.indexOf('"', fromStart);
                        msg["from_username"] = listStr.mid(fromStart, fromEnd - fromStart);
                    }
                    
                    int msgPos = listStr.indexOf("\"message\":", pos);
                    if (msgPos != -1) {
                        int msgStart = listStr.indexOf('"', msgPos + 10) + 1;
                        int msgEnd = listStr.indexOf('"', msgStart);
                        msg["message"] = listStr.mid(msgStart, msgEnd - msgStart);
                    }
                    
                    int timePos = listStr.indexOf("\"sent_at\":", pos);
                    if (timePos != -1) {
                        int timeStart = listStr.indexOf('"', timePos + 10) + 1;
                        int timeEnd = listStr.indexOf('"', timeStart);
                        msg["sent_at"] = listStr.mid(timeStart, timeEnd - timeStart);
                    }
                    
                    messages.append(msg);
                    int endObj = listStr.indexOf("}", pos);
                    if (endObj == -1) break;
                    pos = endObj + 1;
                }
            }
            emit privateChatHistoryReceived(targetUser, totalCount, offset, messages);
            break;
        }
        
        case S_RESP_CHAT_HISTORY_GROUP: {
            QString groupId = data.value("group_id");
            QString groupName = data.value("group_name", "");
            int totalCount = data.value("total_count", "0").toInt();
            int offset = data.value("offset", "0").toInt();
            QList<QMap<QString, QString>> messages;
            
            // Parse messages array
            int start = jsonStr.indexOf('[');
            int end = jsonStr.lastIndexOf(']');
            if (start != -1 && end != -1) {
                QString listStr = jsonStr.mid(start + 1, end - start - 1);
                int pos = 0;
                while ((pos = listStr.indexOf("{", pos)) != -1) {
                    QMap<QString, QString> msg;
                    
                    // Parse message_id (number, not string)
                    int idPos = listStr.indexOf("\"message_id\":", pos);
                    if (idPos != -1) {
                        int idStart = idPos + 13;
                        int idEnd = listStr.indexOf(',', idStart);
                        if (idEnd != -1) {
                            msg["message_id"] = listStr.mid(idStart, idEnd - idStart).trimmed();
                        }
                    }
                    
                    int fromPos = listStr.indexOf("\"from_username\":", pos);
                    if (fromPos != -1) {
                        int fromStart = listStr.indexOf('"', fromPos + 16) + 1;
                        int fromEnd = listStr.indexOf('"', fromStart);
                        msg["from_username"] = listStr.mid(fromStart, fromEnd - fromStart);
                    }
                    
                    int msgPos = listStr.indexOf("\"message\":", pos);
                    if (msgPos != -1) {
                        int msgStart = listStr.indexOf('"', msgPos + 10) + 1;
                        int msgEnd = listStr.indexOf('"', msgStart);
                        msg["message"] = listStr.mid(msgStart, msgEnd - msgStart);
                    }
                    
                    int timePos = listStr.indexOf("\"sent_at\":", pos);
                    if (timePos != -1) {
                        int timeStart = listStr.indexOf('"', timePos + 10) + 1;
                        int timeEnd = listStr.indexOf('"', timeStart);
                        msg["sent_at"] = listStr.mid(timeStart, timeEnd - timeStart);
                    }
                    
                    messages.append(msg);
                    int endObj = listStr.indexOf("}", pos);
                    if (endObj == -1) break;
                    pos = endObj + 1;
                }
            }
            emit groupChatHistoryReceived(groupId, groupName, totalCount, offset, messages);
            break;
        }
            
        case S_RESP_FRIEND_LIST: {
            QList<QPair<QString, bool>> friends;
            int start = jsonStr.indexOf('[');
            int end = jsonStr.lastIndexOf(']');
            if (start != -1 && end != -1) {
                QString listStr = jsonStr.mid(start + 1, end - start - 1);
                int pos = 0;
                while ((pos = listStr.indexOf("{\"username\":", pos)) != -1) {
                    int nameStart = listStr.indexOf('"', pos + 12) + 1;
                    int nameEnd = listStr.indexOf('"', nameStart);
                    QString username = listStr.mid(nameStart, nameEnd - nameStart);
                    
                    int onlinePos = listStr.indexOf("\"online\":", nameEnd);
                    bool isOnline = listStr.indexOf("true", onlinePos) == onlinePos + 9;
                    
                    friends.append(qMakePair(username, isOnline));
                    pos = nameEnd + 1;
                }
            }
            emit friendListReceived(friends);
            break;
        }
            
        case S_RESP_PENDING_REQUESTS: {
            QStringList requests;
            int start = jsonStr.indexOf('[');
            int end = jsonStr.lastIndexOf(']');
            if (start != -1 && end != -1) {
                QString listStr = jsonStr.mid(start + 1, end - start - 1);
                int pos = 0;
                while ((pos = listStr.indexOf('"', pos)) != -1) {
                    int endPos = listStr.indexOf('"', pos + 1);
                    if (endPos != -1) {
                        QString name = listStr.mid(pos + 1, endPos - pos - 1);
                        if (!name.isEmpty() && name != ",") {
                            requests.append(name);
                        }
                        pos = endPos + 1;
                    } else {
                        break;
                    }
                }
            }
            emit pendingRequestsReceived(requests);
            break;
        }
            
        case S_NOTIFY_MSG_PRIVATE: {
            int msgId = data.value("message_id", "0").toInt();
            emit privateMessageReceived(data.value("from_username"), data.value("message"), msgId);
            break;
        }
            
        case S_RESP_PRIVATE_MSG: {
            int msgId = data.value("message_id", "0").toInt();
            emit privateMessageSent(msgId, data.value("target_username"));
            break;
        }
            
        case S_NOTIFY_MSG_GROUP: {
            int msgId = data.value("message_id", "0").toInt();
            emit groupMessageReceived(data.value("group_id"), data.value("group_name"),
                                     data.value("from_username"), data.value("message"), msgId);
            break;
        }
            
        case S_RESP_GROUP_MSG: {
            int msgId = data.value("message_id", "0").toInt();
            emit groupMessageSent(msgId, data.value("group_id"));
            break;
        }
            
        case S_NOTIFY_FRIEND_REQ:
            emit friendRequestReceived(data.value("from_username"));
            break;
            
        case S_NOTIFY_FRIEND_ACCEPT:
            emit friendAccepted(data.value("username"));
            break;
            
        case S_NOTIFY_FRIEND_ONLINE:
            emit friendOnline(data.value("username"));
            break;
            
        case S_NOTIFY_FRIEND_OFFLINE:
            emit friendOffline(data.value("username"));
            break;
            
        case S_NOTIFY_MESSAGES_READ:
            emit messagesReadNotification(data.value("reader_username"));
            break;
            
        case S_NOTIFY_GROUP_JOIN:
            emit userJoinedGroup(data.value("group_id"), data.value("username"));
            break;
            
        case S_NOTIFY_GROUP_LEAVE:
            emit userLeftGroup(data.value("group_id"), data.value("username"));
            break;
            
        case S_RESP_GROUP_INVITE: {
            bool success = (header.status == STATUS_OK);
            emit groupInviteResponse(success, data.value("message"));
            break;
        }
    }
}

// Send methods
void NetworkClient::sendLogin(const QString &username, const QString &password)
{
    QMap<QString, QString> body;
    body["username"] = username;
    body["pass_hash"] = password;  // Server expects "pass_hash"
    sendPacket(C_REQ_LOGIN, body);
}

void NetworkClient::sendRegister(const QString &username, const QString &password)
{
    QMap<QString, QString> body;
    body["username"] = username;
    body["pass_hash"] = password;  // Server expects "pass_hash"
    sendPacket(C_REQ_REGISTER, body);
}

void NetworkClient::sendPrivateMessage(const QString &target, const QString &message)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["target_username"] = target;
    body["message"] = message;
    sendPacket(C_REQ_MSG_PRIVATE, body);
}

void NetworkClient::sendGroupMessage(const QString &groupId, const QString &message)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_id"] = groupId;
    body["message"] = message;
    sendPacket(C_REQ_MSG_GROUP, body);
}

void NetworkClient::sendCreateGroup(const QString &groupName)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_name"] = groupName;
    sendPacket(C_REQ_GROUP_CREATE, body);
}

void NetworkClient::sendJoinGroup(const QString &groupId)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_id"] = groupId;
    sendPacket(C_REQ_GROUP_JOIN, body);
}

void NetworkClient::sendLeaveGroup(const QString &groupId)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_id"] = groupId;
    sendPacket(C_REQ_GROUP_LEAVE, body);
}

void NetworkClient::sendGroupList()
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    sendPacket(C_REQ_GROUP_LIST, body);
}

void NetworkClient::sendAllGroups()
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    sendPacket(C_REQ_ALL_GROUPS, body);
}

void NetworkClient::sendGroupMembers(const QString &groupId)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_id"] = groupId;
    sendPacket(C_REQ_GROUP_MEMBERS, body);
}

void NetworkClient::sendFriendRequest(const QString &username)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["target_username"] = username;
    sendPacket(C_REQ_FRIEND_ADD, body);
}

void NetworkClient::sendFriendResponse(const QString &fromUser, bool accept)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["from_username"] = fromUser;
    body["action"] = accept ? "accept" : "reject";
    sendPacket(C_RESP_FRIEND_REQ, body);
}

void NetworkClient::sendFriendList()
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    sendPacket(C_REQ_FRIEND_LIST, body);
}

void NetworkClient::sendPendingRequests()
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    sendPacket(C_REQ_PENDING_REQUESTS, body);
}

void NetworkClient::sendUnfriend(const QString &username)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["friend_username"] = username;
    sendPacket(C_REQ_UNFRIEND, body);
}

void NetworkClient::sendChangePassword(const QString &oldPassword, const QString &newPassword)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["old_password"] = oldPassword;
    body["new_password"] = newPassword;
    sendPacket(C_REQ_CHANGE_PASS, body);
}

void NetworkClient::sendChatHistoryPrivate(const QString &targetUsername, int offset, int limit)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["target_username"] = targetUsername;
    body["offset"] = QString::number(offset);
    body["limit"] = QString::number(limit);
    sendPacket(C_REQ_CHAT_HISTORY_PRIVATE, body);
}

void NetworkClient::sendChatHistoryGroup(const QString &groupId, int offset, int limit)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_id"] = groupId;
    body["offset"] = QString::number(offset);
    body["limit"] = QString::number(limit);
    sendPacket(C_REQ_CHAT_HISTORY_GROUP, body);
}

void NetworkClient::sendMarkMessagesRead(const QString &senderUsername)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["sender_username"] = senderUsername;
    sendPacket(C_REQ_MARK_MESSAGES_READ, body);
}

void NetworkClient::sendFileUpload(const QString &target, bool isGroup, 
                                    const QString &fileName, qint64 fileSize, 
                                    const QByteArray &fileData)
{
    // Check if file is too large for JSON packet
    if (fileData.size() > 500 * 1024) { // 500KB limit for safety
        qDebug() << "File too large for packet:" << fileData.size() << "bytes";
        return;
    }
    
    // Encode file data as base64
    QString fileDataBase64 = QString::fromLatin1(fileData.toBase64());
    qDebug() << "File upload:" << fileName << fileSize << "bytes, base64 size:" << fileDataBase64.length();
    
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["file_name"] = fileName;
    body["file_size"] = QString::number(fileSize);
    body["file_data"] = fileDataBase64;
    
    if (isGroup) {
        body["group_id"] = target;
    } else {
        body["target_username"] = target;
    }
    
    sendPacket(C_REQ_FILE_UPLOAD, body);
}

void NetworkClient::sendFileDownload(const QString &fileName)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["file_name"] = fileName;
    sendPacket(C_REQ_FILE_DOWNLOAD, body);
}

void NetworkClient::sendDeleteMessage(int messageId, const QString &chatType)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["message_id"] = QString::number(messageId);
    body["chat_type"] = chatType;  // "private" hoặc "group"
    sendPacket(C_REQ_DELETE_MESSAGE, body);
}

void NetworkClient::sendGroupInvite(const QString &groupId, const QString &username)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["group_id"] = groupId;
    body["username"] = username;
    sendPacket(C_REQ_GROUP_INVITE, body);
}
