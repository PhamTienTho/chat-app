#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QMap>
#include <QString>
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
    void sendGroupMembers(const QString &groupId);
    void sendFriendRequest(const QString &username);
    void sendFriendResponse(const QString &fromUser, bool accept);
    void sendFriendList();
    void sendPendingRequests();
    void sendUnfriend(const QString &username);
    void sendChangePassword(const QString &oldPassword, const QString &newPassword);
    
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
    void groupMembersReceived(const QString &groupId, const QString &groupName, 
                              const QList<QPair<QString, bool>> &members);
    void friendListReceived(const QList<QPair<QString, bool>> &friends);
    void pendingRequestsReceived(const QStringList &requests);
    
    // Notification signals
    void privateMessageReceived(const QString &from, const QString &message);
    void groupMessageReceived(const QString &groupId, const QString &groupName, 
                              const QString &from, const QString &message);
    void friendRequestReceived(const QString &from);
    void friendAccepted(const QString &username);
    void friendOnline(const QString &username);
    void friendOffline(const QString &username);
    void userJoinedGroup(const QString &groupId, const QString &username);
    void userLeftGroup(const QString &groupId, const QString &username);

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
