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
#include <QScrollArea>

class NetworkClient;
class QFrame;

// Custom widget for message bubble
class MessageBubble : public QWidget
{
    Q_OBJECT
public:
    MessageBubble(const QString &sender, const QString &message, const QString &time, 
                  bool isMe, QWidget *parent = nullptr);
    void setSeenStatus(bool seen);
    
private:
    bool m_isMe;
    QLabel *m_seenLabel;
};

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
    void onUnfriend();
    void onAttachFile();
    
public slots:
    void downloadFile(const QString &fileName);
    
private slots:
    void onViewGroupMembers();
    void onLeaveCurrentGroup();
    void onGroupMembersReceived(const QString &groupId, const QString &groupName,
                                const QList<QPair<QString, bool>> &members);
    
    // Network responses
    void onFriendListReceived(const QList<QPair<QString, bool>> &friends);
    void onGroupListReceived(const QStringList &groups);
    void onAllGroupsReceived(const QList<QPair<QString, QString>> &groups);
    void onPendingRequestsReceived(const QStringList &requests);
    void onPrivateMessage(const QString &from, const QString &message);
    void onGroupMessage(const QString &groupId, const QString &groupName,
                       const QString &from, const QString &message);
    void onFriendRequest(const QString &from);
    void onFriendAccepted(const QString &username);
    void onFriendOnline(const QString &username);
    void onFriendOffline(const QString &username);
    
    // Chat history slots
    void onLoadMoreMessages();
    void onPrivateChatHistoryReceived(const QString &targetUsername, int totalCount, int offset,
                                       const QList<QMap<QString, QString>> &messages);
    void onGroupChatHistoryReceived(const QString &groupId, const QString &groupName,
                                     int totalCount, int offset,
                                     const QList<QMap<QString, QString>> &messages);
    
    // Read status slot
    void onMessagesReadNotification(const QString &readerUsername);
    void onFileDownloadReceived(const QString &fileName, const QByteArray &fileData, qint64 fileSize);

private:
    void setupUI();
    void setupEmojiPicker();
    void appendMessage(const QString &sender, const QString &message, bool isMe = false);
    void prependMessage(const QString &sender, const QString &message, const QString &time, bool isMe = false);
    void showNotification(const QString &title, const QString &message);
    void onEmojiClicked(const QString &emoji);
    void toggleEmojiPicker();
    void loadChatHistory();
    QString formatFileSize(qint64 bytes);
    
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
    QListWidget *m_chatListWidget;  // Changed from QTextEdit to QListWidget
    QLabel *m_seenStatusLabel;  // Label hiển thị "Đã xem"
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QToolButton *m_emojiButton;
    QToolButton *m_attachButton;
    QFrame *m_emojiPicker;
    QPushButton *m_loadMoreBtn;
    
    // Chat history tracking
    int m_currentOffset;
    int m_totalMessageCount;
    
    // Store chat history per target
    QMap<QString, QString> m_chatHistory;
    
    // Track last message for "seen" status per user
    QMap<QString, bool> m_messageSeenStatus;  // target -> whether they've seen our last message
    
    // File download path
    QString m_downloadPath;
};

#endif // CHATWIDGET_H
