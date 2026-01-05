#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QMap>
#include <QToolButton>
#include <QMenu>
#include <QGridLayout>

class NetworkClient;
class QFrame;

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(NetworkClient *client, const QString &username, QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void onSendClicked();
    void onLogout();
    void onChangePassword();
    void onChangePasswordResponse(bool success, const QString &message);
    void onFriendSelected(QListWidgetItem *item);
    void onGroupSelected(QListWidgetItem *item);
    
    // Button actions
    void onRefreshFriends();
    void onRefreshGroups();
    void onAddFriend();
    void onCreateGroup();
    void onJoinGroup();
    void onLeaveGroup();
    void onViewPendingRequests();
    
    // Group menu actions
    void onViewGroupMembers();
    void onLeaveCurrentGroup();
    void onGroupMembersReceived(const QString &groupId, const QString &groupName,
                                const QList<QPair<QString, bool>> &members);
    
    // Network responses
    void onFriendListReceived(const QList<QPair<QString, bool>> &friends);
    void onGroupListReceived(const QStringList &groups);
    void onPendingRequestsReceived(const QStringList &requests);
    void onPrivateMessage(const QString &from, const QString &message);
    void onGroupMessage(const QString &groupId, const QString &groupName,
                       const QString &from, const QString &message);
    void onFriendRequest(const QString &from);
    void onFriendAccepted(const QString &username);
    void onFriendOnline(const QString &username);
    void onFriendOffline(const QString &username);

private:
    void setupUI();
    void setupEmojiPicker();
    void appendMessage(const QString &sender, const QString &message, bool isMe = false);
    void showNotification(const QString &title, const QString &message);
    void onEmojiClicked(const QString &emoji);
    void toggleEmojiPicker();
    
    NetworkClient *m_client;
    QString m_username;
    
    // Current chat target
    QString m_currentTarget;  // Username or group_id
    QString m_currentGroupName;  // Store group name for display
    bool m_isChatWithGroup;
    
    // UI Components
    QTabWidget *m_sidebarTabs;
    QListWidget *m_friendsList;
    QListWidget *m_groupsList;
    
    QLabel *m_chatHeader;
    QToolButton *m_groupMenuBtn;
    QMenu *m_groupMenu;
    QTextEdit *m_chatDisplay;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QToolButton *m_emojiButton;
    QFrame *m_emojiPicker;
    
    // Store chat history per target
    QMap<QString, QString> m_chatHistory;
};

#endif // CHATWIDGET_H
