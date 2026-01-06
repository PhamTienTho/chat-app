#include "networkclient.h"
#include <QDataStream>
#include <QDebug>

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
            
        case S_NOTIFY_MSG_PRIVATE:
            emit privateMessageReceived(data.value("from_username"), data.value("message"));
            break;
            
        case S_NOTIFY_MSG_GROUP:
            emit groupMessageReceived(data.value("group_id"), data.value("group_name"),
                                     data.value("from_username"), data.value("message"));
            break;
            
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
            
        case S_NOTIFY_GROUP_JOIN:
            emit userJoinedGroup(data.value("group_id"), data.value("username"));
            break;
            
        case S_NOTIFY_GROUP_LEAVE:
            emit userLeftGroup(data.value("group_id"), data.value("username"));
            break;
            
        case S_RESP_CHAT_HISTORY_PRIVATE:
        {
            QString targetUsername = data.value("target_username");
            int totalCount = data.value("total_count").toInt();
            int offset = data.value("offset").toInt();
            
            // Parse messages array from raw JSON
            QString jsonStr = QString::fromUtf8(body);
            QList<QMap<QString, QString>> messages;
            
            // Simple parsing for messages array
            int msgStart = jsonStr.indexOf("\"messages\":[");
            if (msgStart != -1) {
                int arrStart = jsonStr.indexOf('[', msgStart);
                int arrEnd = jsonStr.lastIndexOf(']');
                QString msgArr = jsonStr.mid(arrStart + 1, arrEnd - arrStart - 1);
                
                // Parse each message object
                int pos = 0;
                while ((pos = msgArr.indexOf('{', pos)) != -1) {
                    int endPos = msgArr.indexOf('}', pos);
                    if (endPos == -1) break;
                    QString msgObj = msgArr.mid(pos, endPos - pos + 1);
                    
                    QMap<QString, QString> msg;
                    // Extract from_username
                    int idx = msgObj.indexOf("\"from_username\":\"");
                    if (idx != -1) {
                        int start = idx + 17;
                        int end = msgObj.indexOf('"', start);
                        msg["from_username"] = msgObj.mid(start, end - start);
                    }
                    // Extract message
                    idx = msgObj.indexOf("\"message\":\"");
                    if (idx != -1) {
                        int start = idx + 11;
                        int end = msgObj.indexOf("\",", start);
                        if (end == -1) end = msgObj.indexOf("\"}", start);
                        msg["message"] = msgObj.mid(start, end - start);
                    }
                    // Extract sent_at
                    idx = msgObj.indexOf("\"sent_at\":\"");
                    if (idx != -1) {
                        int start = idx + 11;
                        int end = msgObj.indexOf('"', start);
                        msg["sent_at"] = msgObj.mid(start, end - start);
                    }
                    
                    if (!msg.isEmpty()) {
                        messages.append(msg);
                    }
                    pos = endPos + 1;
                }
            }
            
            emit privateChatHistoryReceived(targetUsername, totalCount, offset, messages);
            break;
        }
        
        case S_RESP_CHAT_HISTORY_GROUP:
        {
            QString groupId = data.value("group_id");
            QString groupName = data.value("group_name");
            int totalCount = data.value("total_count").toInt();
            int offset = data.value("offset").toInt();
            
            // Parse messages array from raw JSON
            QString jsonStr = QString::fromUtf8(body);
            QList<QMap<QString, QString>> messages;
            
            int msgStart = jsonStr.indexOf("\"messages\":[");
            if (msgStart != -1) {
                int arrStart = jsonStr.indexOf('[', msgStart);
                int arrEnd = jsonStr.lastIndexOf(']');
                QString msgArr = jsonStr.mid(arrStart + 1, arrEnd - arrStart - 1);
                
                int pos = 0;
                while ((pos = msgArr.indexOf('{', pos)) != -1) {
                    int endPos = msgArr.indexOf('}', pos);
                    if (endPos == -1) break;
                    QString msgObj = msgArr.mid(pos, endPos - pos + 1);
                    
                    QMap<QString, QString> msg;
                    int idx = msgObj.indexOf("\"from_username\":\"");
                    if (idx != -1) {
                        int start = idx + 17;
                        int end = msgObj.indexOf('"', start);
                        msg["from_username"] = msgObj.mid(start, end - start);
                    }
                    idx = msgObj.indexOf("\"message\":\"");
                    if (idx != -1) {
                        int start = idx + 11;
                        int end = msgObj.indexOf("\",", start);
                        if (end == -1) end = msgObj.indexOf("\"}", start);
                        msg["message"] = msgObj.mid(start, end - start);
                    }
                    idx = msgObj.indexOf("\"sent_at\":\"");
                    if (idx != -1) {
                        int start = idx + 11;
                        int end = msgObj.indexOf('"', start);
                        msg["sent_at"] = msgObj.mid(start, end - start);
                    }
                    
                    if (!msg.isEmpty()) {
                        messages.append(msg);
                    }
                    pos = endPos + 1;
                }
            }
            
            emit groupChatHistoryReceived(groupId, groupName, totalCount, offset, messages);
            break;
        }
        
        case S_NOTIFY_MESSAGES_READ:
        {
            QString readerUsername = data.value("reader_username");
            emit messagesReadNotification(readerUsername);
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

void NetworkClient::sendMarkMessagesRead(const QString &fromUsername)
{
    QMap<QString, QString> body;
    body["token"] = m_token;
    body["from_username"] = fromUsername;
    sendPacket(C_REQ_MARK_MESSAGES_READ, body);
}
