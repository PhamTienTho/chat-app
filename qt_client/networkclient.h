#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include "protocol.h"

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);
    ~NetworkClient();
    
    bool connectToServer(const QString &host, int port);
    void disconnectFromServer();
    bool isConnected() const;
    
    // Send packets
    void sendLogin(const QString &username, const QString &password);
    void sendRegister(const QString &username, const QString &password);
    void sendPrivateMessage(const QString &target, const QString &message);
    void sendGroupMessage(const QString &groupId, const QString &message);
    void sendCreateGroup(const QString &groupName);
    void sendJoinGroup(const QString &groupId);
    void sendLeaveGroup(const QString &groupId);
    void sendGroupList();
    void sendAllGroups();   // Lấy tất cả nhóm
    void sendAllUsers();    // Lấy tất cả users
    void sendGroupMembers(const QString &groupId);
    void sendFriendRequest(const QString &username);
    void sendFriendResponse(const QString &fromUser, bool accept);
    void sendFriendList();
    void sendPendingRequests();
    void sendUnfriend(const QString &username);
    void sendChangePassword(const QString &oldPassword, const QString &newPassword);
    void sendChatHistoryPrivate(const QString &targetUsername, int offset = 0, int limit = 10);
    void sendChatHistoryGroup(const QString &groupId, int offset = 0, int limit = 10);
    void sendMarkMessagesRead(const QString &senderUsername);
    void sendFileUpload(const QString &target, bool isGroup, const QString &fileName, 
                        qint64 fileSize, const QByteArray &fileData);
    void sendFileDownload(const QString &fileName);
    void sendDeleteMessage(int messageId, const QString &chatType);  // "private" hoặc "group"
    
    void setToken(const QString &token) { m_token = token; }
    QString getToken() const { return m_token; }

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    
    // Response signals
    void loginResponse(bool success, const QString &message, const QString &token);
    void registerResponse(bool success, const QString &message);
    void changePasswordResponse(bool success, const QString &message);
    void groupCreated(bool success, const QString &message, const QString &groupId);
    void groupListReceived(const QStringList &groups);
    void allGroupsReceived(const QList<QPair<QString, QString>> &groups); // groupId, display
    void allGroupsListSimple(const QStringList &items); // "groupId|Group Name (N members)"
    void groupMembersReceived(const QString &groupId, const QString &groupName, 
                              const QList<QPair<QString, bool>> &members);
    void friendListReceived(const QList<QPair<QString, bool>> &friends);
    void allUsersReceived(const QList<QPair<QString, QString>> &users);
    void pendingRequestsReceived(const QStringList &requests);
    
    // Notifications
    void privateMessageReceived(const QString &from, const QString &message, int messageId);
    void privateMessageSent(int messageId, const QString &targetUsername);  // Xác nhận tin nhắn private đã gửi
    void groupMessageReceived(const QString &groupId, const QString &groupName, 
                              const QString &from, const QString &message, int messageId);
    void groupMessageSent(int messageId, const QString &groupId);  // Xác nhận tin nhắn nhóm đã gửi
    void friendRequestReceived(const QString &from);
    void friendAccepted(const QString &username);
    void friendOnline(const QString &username);
    void friendOffline(const QString &username);
    void userJoinedGroup(const QString &groupId, const QString &username);
    void userLeftGroup(const QString &groupId, const QString &username);

    // Chat history
    void privateChatHistoryReceived(const QString &targetUsername, int totalCount, int offset,
                                    const QList<QMap<QString, QString>> &messages);
    void groupChatHistoryReceived(const QString &groupId, const QString &groupName,
                                  int totalCount, int offset,
                                  const QList<QMap<QString, QString>> &messages);

    // Read status
    void messagesReadNotification(const QString &readerUsername);
    
    // File operations
    void fileDownloadReceived(const QString &fileName, const QByteArray &fileData, qint64 fileSize);
    
    // Delete message
    void deleteMessageResponse(bool success, const QString &message, int messageId);
    void messageDeleted(int messageId, const QString &chatType, const QString &groupId);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);

private:
    void sendPacket(int command, const QMap<QString, QString> &body);
    void processPacket(const PacketHeader &header, const QByteArray &body);
    QString buildJson(const QMap<QString, QString> &data);
    QMap<QString, QString> parseJson(const QString &json);
    
    QTcpSocket *m_socket;
    QString m_token;
    QByteArray m_buffer;
};

#endif // NETWORKCLIENT_H
