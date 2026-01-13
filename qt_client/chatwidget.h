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
                  bool isMe, int messageId = -1, QWidget *parent = nullptr);
    void setSeenStatus(bool seen);
    int getMessageId() const { return m_messageId; }
    void setMessageId(int id) { m_messageId = id; }
    bool isMine() const { return m_isMe; }
    QString getMessage() const { return m_message; }
    
signals:
    void deleteRequested(int messageId);
    
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    
private:
    bool m_isMe;
    int m_messageId;
    QString m_message;
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
    void onInviteMemberToGroup();
    void onGroupInviteResponse(bool success, const QString &message);
    void onGroupMembersReceived(const QString &groupId, const QString &groupName,
                                const QList<QPair<QString, bool>> &members);
    
    // Network responses
    void onFriendListReceived(const QList<QPair<QString, bool>> &friends);
    void onGroupListReceived(const QStringList &groups);
    void onAllGroupsReceived(const QList<QPair<QString, QString>> &groups);
    void onPendingRequestsReceived(const QStringList &requests);
    void onPrivateMessage(const QString &from, const QString &message, int messageId);
    void onGroupMessage(const QString &groupId, const QString &groupName,
                       const QString &from, const QString &message, int messageId);
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
    
    // Delete message slots
    void onDeleteMessageRequested(int messageId);
    void onDeleteMessageResponse(bool success, const QString &message, int messageId);
    void onMessageDeleted(int messageId, const QString &chatType, const QString &groupId);
    
    // Message sent confirmation slots
    void onPrivateMessageSent(int messageId, const QString &targetUsername);
    void onGroupMessageSent(int messageId, const QString &groupId);
    
    // Search slots
    void onSearchToggle();
    void onSearchTextChanged(const QString &text);
    void onSearchNext();
    void onSearchPrev();
    void onSearchClose();

private:
    void setupUI();
    void setupEmojiPicker();
    void setupSearchBar();
    void appendMessage(const QString &sender, const QString &message, bool isMe = false, int messageId = -1);
    void prependMessage(const QString &sender, const QString &message, const QString &time, bool isMe = false);
    void showNotification(const QString &title, const QString &message);
    void onEmojiClicked(const QString &emoji);
    void toggleEmojiPicker();
    void loadChatHistory();
    QString formatFileSize(qint64 bytes);
    void removeMessageBubbleById(int messageId);
    void highlightSearchResult();
    
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
    
    // Track last sent message bubble (for updating message_id)
    MessageBubble *m_lastSentBubble;
    
    // Store current group members for invite feature
    QStringList m_currentGroupMembers;
    
    // Flag to show group members dialog
    bool m_showGroupMembersDialog;
    
    // Search components
    QToolButton *m_searchBtn;
    QFrame *m_searchBar;
    QLineEdit *m_searchInput;
    QLabel *m_searchResultLabel;
    QPushButton *m_searchPrevBtn;
    QPushButton *m_searchNextBtn;
    QPushButton *m_searchCloseBtn;
    QList<int> m_searchResults;  // Indices of matching messages in chat list
    int m_currentSearchIndex;
};

#endif // CHATWIDGET_H
