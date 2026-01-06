#include "chatwidget.h"
#include "networkclient.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QScrollBar>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QBrush>
#include <QColor>
#include <QInputMethod>
#include <QGuiApplication>
#include <QDialog>
#include <QFrame>
#include <QGridLayout>
#include <QScrollArea>

// ===== MessageBubble Implementation =====
MessageBubble::MessageBubble(const QString &sender, const QString &message, const QString &time, 
                             bool isMe, QWidget *parent)
    : QWidget(parent), m_isMe(isMe), m_seenLabel(nullptr)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    
    // Create bubble widget
    QWidget *bubble = new QWidget;
    QVBoxLayout *bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->setContentsMargins(14, 10, 14, 10);
    bubbleLayout->setSpacing(4);
    
    // Sender name
    QLabel *senderLabel = new QLabel(sender);
    senderLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 14px;")
                               .arg(isMe ? "#075E54" : "#128C7E"));
    
    // Message content
    QLabel *messageLabel = new QLabel(message);
    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet("color: #1a1a1a; font-size: 15px;");
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    
    // Time
    QLabel *timeLabel = new QLabel(time);
    timeLabel->setStyleSheet("color: #888; font-size: 11px;");
    timeLabel->setAlignment(isMe ? Qt::AlignRight : Qt::AlignLeft);
    
    bubbleLayout->addWidget(senderLabel);
    bubbleLayout->addWidget(messageLabel);
    bubbleLayout->addWidget(timeLabel);
    
    // Seen status (only for own messages)
    if (isMe) {
        m_seenLabel = new QLabel("");
        m_seenLabel->setStyleSheet("color: #34B7F1; font-size: 11px;");
        m_seenLabel->setAlignment(Qt::AlignRight);
        m_seenLabel->setVisible(false);
        bubbleLayout->addWidget(m_seenLabel);
    }
    
    // Style the bubble
    if (isMe) {
        bubble->setStyleSheet(
            "background-color: #DCF8C6; "
            "border-radius: 18px; "
            "border-top-right-radius: 5px;"
        );
        mainLayout->addStretch(1);
        mainLayout->addWidget(bubble, 3);
    } else {
        bubble->setStyleSheet(
            "background-color: #E8E8E8; "
            "border-radius: 18px; "
            "border-top-left-radius: 5px;"
        );
        mainLayout->addWidget(bubble, 3);
        mainLayout->addStretch(1);
    }
    
    bubble->setMaximumWidth(500);
}

void MessageBubble::setSeenStatus(bool seen)
{
    if (m_seenLabel && m_isMe) {
        m_seenLabel->setText(seen ? "✓✓ Đã xem" : "");
        m_seenLabel->setVisible(seen);
    }
}
// ===== End MessageBubble =====

ChatWidget::ChatWidget(NetworkClient *client, const QString &username, QWidget *parent)
    : QWidget(parent), m_client(client), m_username(username), m_isChatWithGroup(false)
{
    setupUI();
    
    // Connect signals
    connect(m_client, &NetworkClient::friendListReceived, this, &ChatWidget::onFriendListReceived);
    connect(m_client, &NetworkClient::groupListReceived, this, &ChatWidget::onGroupListReceived);
    connect(m_client, &NetworkClient::pendingRequestsReceived, this, &ChatWidget::onPendingRequestsReceived);
    connect(m_client, &NetworkClient::privateMessageReceived, this, &ChatWidget::onPrivateMessage);
    connect(m_client, &NetworkClient::groupMessageReceived, this, &ChatWidget::onGroupMessage);
    connect(m_client, &NetworkClient::friendRequestReceived, this, &ChatWidget::onFriendRequest);
    connect(m_client, &NetworkClient::friendAccepted, this, &ChatWidget::onFriendAccepted);
    connect(m_client, &NetworkClient::friendOnline, this, &ChatWidget::onFriendOnline);
    connect(m_client, &NetworkClient::friendOffline, this, &ChatWidget::onFriendOffline);
    connect(m_client, &NetworkClient::groupMembersReceived, this, &ChatWidget::onGroupMembersReceived);
    connect(m_client, &NetworkClient::changePasswordResponse, this, &ChatWidget::onChangePasswordResponse);
    connect(m_client, &NetworkClient::privateChatHistoryReceived, this, &ChatWidget::onPrivateChatHistoryReceived);
    connect(m_client, &NetworkClient::groupChatHistoryReceived, this, &ChatWidget::onGroupChatHistoryReceived);
    connect(m_client, &NetworkClient::messagesReadNotification, this, &ChatWidget::onMessagesReadNotification);
    
    // Initialize tracking variables
    m_currentOffset = 0;
    m_totalMessageCount = 0;
    
    // Initial data load
    m_client->sendFriendList();
    m_client->sendGroupList();
}

void ChatWidget::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left sidebar
    QWidget *sidebar = new QWidget;
    sidebar->setFixedWidth(280);
    sidebar->setStyleSheet("background-color: #f5f5f5;");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    
    // User info
    QLabel *userLabel = new QLabel("User: " + m_username);
    userLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; "
                            "background-color: #2196F3; color: white;");
    sidebarLayout->addWidget(userLabel);
    
    // Tabs for friends and groups
    m_sidebarTabs = new QTabWidget;
    
    // Friends tab
    QWidget *friendsTab = new QWidget;
    QVBoxLayout *friendsLayout = new QVBoxLayout(friendsTab);
    friendsLayout->setContentsMargins(8, 10, 8, 5);
    friendsLayout->setSpacing(10);
    
    QHBoxLayout *friendsButtons = new QHBoxLayout;
    friendsButtons->setSpacing(6);
    
    QPushButton *refreshFriendsBtn = new QPushButton("Refresh");
    refreshFriendsBtn->setToolTip("Làm mới danh sách");
    refreshFriendsBtn->setMinimumHeight(36);
    refreshFriendsBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 8px 12px; "
        "border-radius: 6px; font-weight: bold; font-size: 12px; border: none; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:pressed { background-color: #1565C0; }");
    
    QPushButton *addFriendBtn = new QPushButton("+ Add");
    addFriendBtn->setToolTip("Thêm bạn");
    addFriendBtn->setMinimumHeight(36);
    addFriendBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 8px 12px; "
        "border-radius: 6px; font-weight: bold; font-size: 12px; border: none; }"
        "QPushButton:hover { background-color: #43A047; }"
        "QPushButton:pressed { background-color: #388E3C; }");
    
    QPushButton *pendingBtn = new QPushButton("Pending");
    pendingBtn->setToolTip("Lời mời chờ");
    pendingBtn->setMinimumHeight(36);
    pendingBtn->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; padding: 8px 12px; "
        "border-radius: 6px; font-weight: bold; font-size: 12px; border: none; }"
        "QPushButton:hover { background-color: #F57C00; }"
        "QPushButton:pressed { background-color: #E65100; }");
    
    friendsButtons->addWidget(refreshFriendsBtn);
    friendsButtons->addWidget(addFriendBtn);
    friendsButtons->addWidget(pendingBtn);
    friendsLayout->addLayout(friendsButtons);
    
    m_friendsList = new QListWidget;
    m_friendsList->setStyleSheet(
        "QListWidget { background-color: white; border: none; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #eee; }"
        "QListWidget::item:hover { background-color: #e3f2fd; }"
        "QListWidget::item:selected { background-color: #2196F3; color: white; }");
    friendsLayout->addWidget(m_friendsList);
    
    connect(refreshFriendsBtn, &QPushButton::clicked, this, &ChatWidget::onRefreshFriends);
    connect(addFriendBtn, &QPushButton::clicked, this, &ChatWidget::onAddFriend);
    connect(pendingBtn, &QPushButton::clicked, this, &ChatWidget::onViewPendingRequests);
    connect(m_friendsList, &QListWidget::itemClicked, this, &ChatWidget::onFriendSelected);
    
    m_sidebarTabs->addTab(friendsTab, "Friends");
    
    // Groups tab
    QWidget *groupsTab = new QWidget;
    QVBoxLayout *groupsLayout = new QVBoxLayout(groupsTab);
    groupsLayout->setContentsMargins(8, 10, 8, 5);
    groupsLayout->setSpacing(10);
    
    QHBoxLayout *groupsButtons = new QHBoxLayout;
    groupsButtons->setSpacing(6);
    
    QPushButton *refreshGroupsBtn = new QPushButton("Refresh");
    refreshGroupsBtn->setToolTip("Làm mới");
    refreshGroupsBtn->setMinimumHeight(36);
    refreshGroupsBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 8px 10px; "
        "border-radius: 6px; font-weight: bold; font-size: 11px; border: none; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:pressed { background-color: #1565C0; }");
    
    QPushButton *createGroupBtn = new QPushButton("+ New");
    createGroupBtn->setToolTip("Tạo nhóm");
    createGroupBtn->setMinimumHeight(36);
    createGroupBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 8px 10px; "
        "border-radius: 6px; font-weight: bold; font-size: 11px; border: none; }"
        "QPushButton:hover { background-color: #43A047; }"
        "QPushButton:pressed { background-color: #388E3C; }");
    
    QPushButton *joinGroupBtn = new QPushButton("Join");
    joinGroupBtn->setToolTip("Tham gia nhóm");
    joinGroupBtn->setMinimumHeight(36);
    joinGroupBtn->setStyleSheet(
        "QPushButton { background-color: #9C27B0; color: white; padding: 8px 10px; "
        "border-radius: 6px; font-weight: bold; font-size: 11px; border: none; }"
        "QPushButton:hover { background-color: #7B1FA2; }"
        "QPushButton:pressed { background-color: #6A1B9A; }");
    
    QPushButton *leaveGroupBtn = new QPushButton("Leave");
    leaveGroupBtn->setToolTip("Rời nhóm");
    leaveGroupBtn->setMinimumHeight(36);
    leaveGroupBtn->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; padding: 8px 10px; "
        "border-radius: 6px; font-weight: bold; font-size: 11px; border: none; }"
        "QPushButton:hover { background-color: #d32f2f; }"
        "QPushButton:pressed { background-color: #c62828; }");
    
    groupsButtons->addWidget(refreshGroupsBtn);
    groupsButtons->addWidget(createGroupBtn);
    groupsButtons->addWidget(joinGroupBtn);
    groupsButtons->addWidget(leaveGroupBtn);
    groupsLayout->addLayout(groupsButtons);
    
    m_groupsList = new QListWidget;
    m_groupsList->setStyleSheet(
        "QListWidget { background-color: white; border: none; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #eee; }"
        "QListWidget::item:hover { background-color: #e8f5e9; }"
        "QListWidget::item:selected { background-color: #4CAF50; color: white; }");
    groupsLayout->addWidget(m_groupsList);
    
    connect(refreshGroupsBtn, &QPushButton::clicked, this, &ChatWidget::onRefreshGroups);
    connect(createGroupBtn, &QPushButton::clicked, this, &ChatWidget::onCreateGroup);
    connect(joinGroupBtn, &QPushButton::clicked, this, &ChatWidget::onJoinGroup);
    connect(leaveGroupBtn, &QPushButton::clicked, this, &ChatWidget::onLeaveGroup);
    connect(m_groupsList, &QListWidget::itemClicked, this, &ChatWidget::onGroupSelected);
    
    m_sidebarTabs->addTab(groupsTab, "Groups");
    
    // Account tab
    QWidget *accountTab = new QWidget;
    QVBoxLayout *accountLayout = new QVBoxLayout(accountTab);
    accountLayout->setContentsMargins(15, 20, 15, 20);
    accountLayout->setSpacing(15);
    
    QLabel *accountIcon = new QLabel;
    accountIcon->setText("\xF0\x9F\x91\xA4");  // User icon
    accountIcon->setStyleSheet("font-size: 48px; background: transparent;");
    accountIcon->setAlignment(Qt::AlignCenter);
    accountLayout->addWidget(accountIcon);
    
    QLabel *usernameLabel = new QLabel(m_username);
    usernameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333; background: transparent;");
    usernameLabel->setAlignment(Qt::AlignCenter);
    accountLayout->addWidget(usernameLabel);
    
    QLabel *statusLabel = new QLabel("Online");
    statusLabel->setStyleSheet("font-size: 12px; color: #4CAF50; background: transparent;");
    statusLabel->setAlignment(Qt::AlignCenter);
    accountLayout->addWidget(statusLabel);
    
    accountLayout->addStretch();
    
    QPushButton *changePassBtn = new QPushButton("Change Password");
    changePassBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 12px; "
        "border-radius: 6px; font-size: 14px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #1976D2; }");
    connect(changePassBtn, &QPushButton::clicked, this, &ChatWidget::onChangePassword);
    accountLayout->addWidget(changePassBtn);
    
    QPushButton *logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; padding: 12px; "
        "border-radius: 6px; font-size: 14px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #d32f2f; }");
    connect(logoutBtn, &QPushButton::clicked, this, &ChatWidget::onLogout);
    accountLayout->addWidget(logoutBtn);
    
    m_sidebarTabs->addTab(accountTab, "Account");
    
    sidebarLayout->addWidget(m_sidebarTabs);
    
    // Right chat area
    QWidget *chatArea = new QWidget;
    chatArea->setStyleSheet("background-color: white;");
    QVBoxLayout *chatLayout = new QVBoxLayout(chatArea);
    
    // Chat header with menu button
    QWidget *headerWidget = new QWidget;
    headerWidget->setStyleSheet("background-color: #e3f2fd; border-bottom: 1px solid #ddd;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 10, 15, 10);
    
    m_chatHeader = new QLabel("Chon nguoi hoac nhom de chat");
    m_chatHeader->setStyleSheet("font-size: 18px; font-weight: bold;");
    headerLayout->addWidget(m_chatHeader);
    
    headerLayout->addStretch();
    
    // Group menu button (hidden by default) - 3 dots icon
    m_groupMenuBtn = new QToolButton;
    m_groupMenuBtn->setText("\u22EE");  // Vertical ellipsis (3 dots)
    m_groupMenuBtn->setStyleSheet(
        "QToolButton { background-color: transparent; color: #555; "
        "font-size: 24px; font-weight: bold; padding: 5px 10px; "
        "border: none; border-radius: 5px; }"
        "QToolButton:hover { background-color: #ddd; }"
        "QToolButton::menu-indicator { image: none; }");
    m_groupMenuBtn->setPopupMode(QToolButton::InstantPopup);
    m_groupMenuBtn->setVisible(false);  // Hidden until a group is selected
    
    m_groupMenu = new QMenu(this);
    m_groupMenu->setStyleSheet(
        "QMenu { background-color: white; border: 1px solid #ddd; }"
        "QMenu::item { padding: 8px 20px; }"
        "QMenu::item:selected { background-color: #e3f2fd; }");
    QAction *viewMembersAction = m_groupMenu->addAction("View Members");
    m_groupMenu->addSeparator();
    QAction *leaveGroupAction = m_groupMenu->addAction("Leave Group");
    
    m_groupMenuBtn->setMenu(m_groupMenu);
    headerLayout->addWidget(m_groupMenuBtn);
    
    connect(viewMembersAction, &QAction::triggered, this, &ChatWidget::onViewGroupMembers);
    connect(leaveGroupAction, &QAction::triggered, this, &ChatWidget::onLeaveCurrentGroup);
    
    chatLayout->addWidget(headerWidget);
    
    // Load more button (hidden by default)
    m_loadMoreBtn = new QPushButton("📜 Tải tin nhắn cũ hơn...");
    m_loadMoreBtn->setStyleSheet(
        "QPushButton { background-color: #f0f0f0; color: #666; padding: 8px; "
        "border: 1px solid #ddd; border-radius: 8px; font-size: 13px; }"
        "QPushButton:hover { background-color: #e0e0e0; }");
    m_loadMoreBtn->setVisible(false);
    connect(m_loadMoreBtn, &QPushButton::clicked, this, &ChatWidget::onLoadMoreMessages);
    chatLayout->addWidget(m_loadMoreBtn);
    
    // Chat display - using QListWidget for proper bubble styling
    m_chatListWidget = new QListWidget;
    m_chatListWidget->setStyleSheet(
        "QListWidget { border: none; background-color: #f5f5f5; }"
        "QListWidget::item { border: none; background: transparent; }");
    m_chatListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_chatListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_chatListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_chatListWidget->setSpacing(2);
    chatLayout->addWidget(m_chatListWidget);
    
    // Seen status label - hiển thị "Đã xem" bên phải
    m_seenStatusLabel = new QLabel;
    m_seenStatusLabel->setAlignment(Qt::AlignRight);
    m_seenStatusLabel->setStyleSheet(
        "QLabel { color: #34B7F1; font-size: 12px; padding: 2px 15px; }");
    m_seenStatusLabel->setVisible(false);
    chatLayout->addWidget(m_seenStatusLabel);
    
    // Message input area
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->setContentsMargins(10, 10, 10, 10);
    inputLayout->setSpacing(8);
    
    // Emoji button
    m_emojiButton = new QToolButton;
    uint32_t smileyCode = 0x1F60A;
    m_emojiButton->setText(QString::fromUcs4(&smileyCode, 1));
    m_emojiButton->setFont(QFont("Noto Color Emoji", 18));
    m_emojiButton->setStyleSheet(
        "QToolButton { background-color: #f5f5f5; border: 1px solid #ddd; "
        "border-radius: 20px; padding: 8px; min-width: 40px; min-height: 40px; }"
        "QToolButton:hover { background-color: #e0e0e0; }"
        "QToolButton:pressed { background-color: #bdbdbd; }");
    m_emojiButton->setEnabled(false);
    connect(m_emojiButton, &QToolButton::clicked, this, &ChatWidget::toggleEmojiPicker);
    
    m_messageInput = new QLineEdit;
    m_messageInput->setPlaceholderText("Type a message...");
    m_messageInput->setStyleSheet("padding: 12px; border: 1px solid #ddd; border-radius: 20px; font-size: 14px; font-family: 'Noto Color Emoji', 'Segoe UI Emoji', sans-serif;");
    m_messageInput->setEnabled(false);
    m_messageInput->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_messageInput->setInputMethodHints(Qt::ImhNone);
    
    m_sendButton = new QPushButton("Send");
    m_sendButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 12px 25px; "
        "border-radius: 20px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:disabled { background-color: #ccc; }");
    m_sendButton->setEnabled(false);
    
    inputLayout->addWidget(m_emojiButton);
    inputLayout->addWidget(m_messageInput);
    inputLayout->addWidget(m_sendButton);
    chatLayout->addLayout(inputLayout);
    
    // Setup emoji picker (hidden by default)
    setupEmojiPicker();
    
    connect(m_sendButton, &QPushButton::clicked, this, &ChatWidget::onSendClicked);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &ChatWidget::onSendClicked);
    
    // Add to main layout
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(chatArea, 1);
}

void ChatWidget::setupEmojiPicker()
{
    // Create emoji picker popup frame
    m_emojiPicker = new QFrame(this);
    m_emojiPicker->setWindowFlags(Qt::Popup);
    m_emojiPicker->setFrameShape(QFrame::StyledPanel);
    m_emojiPicker->setStyleSheet(
        "QFrame { background-color: white; border: 1px solid #ddd; border-radius: 10px; }"
    );
    
    QVBoxLayout *pickerLayout = new QVBoxLayout(m_emojiPicker);
    pickerLayout->setContentsMargins(10, 10, 10, 10);
    pickerLayout->setSpacing(5);
    
    // Title
    QLabel *titleLabel = new QLabel("Choose Emoji");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333; padding: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    pickerLayout->addWidget(titleLabel);
    
    // Scroll area for emojis
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    scrollArea->setFixedSize(320, 280);
    
    QWidget *emojiContainer = new QWidget;
    QGridLayout *emojiGrid = new QGridLayout(emojiContainer);
    emojiGrid->setSpacing(3);
    emojiGrid->setContentsMargins(5, 5, 5, 5);
    
    // Helper function to create emoji QString from Unicode codepoint
    // Add variation selector (0xFE0F) to force color emoji rendering
    auto makeEmoji = [](uint32_t codepoint, bool addVS = false) -> QString {
        QString emoji = QString::fromUcs4(&codepoint, 1);
        if (addVS) {
            emoji += QChar(0xFE0F);  // Variation Selector-16 for color emoji
        }
        return emoji;
    };
    
    // Emoji list - using direct QString literals for reliability
    QStringList emojis;
    
    // Smileys - these work well with fromUcs4
    emojis << makeEmoji(0x1F600) << makeEmoji(0x1F603) << makeEmoji(0x1F604) << makeEmoji(0x1F601);
    emojis << makeEmoji(0x1F606) << makeEmoji(0x1F605) << makeEmoji(0x1F602) << makeEmoji(0x1F923);
    emojis << makeEmoji(0x1F60A) << makeEmoji(0x1F607) << makeEmoji(0x1F609) << makeEmoji(0x1F60D);
    emojis << makeEmoji(0x1F618) << makeEmoji(0x1F617) << makeEmoji(0x1F619) << makeEmoji(0x1F61A);
    emojis << makeEmoji(0x1F61B) << makeEmoji(0x1F61C) << makeEmoji(0x1F61D) << makeEmoji(0x1F60B);
    emojis << makeEmoji(0x1F911) << makeEmoji(0x1F917) << makeEmoji(0x1F914) << makeEmoji(0x1F910);
    emojis << makeEmoji(0x1F610) << makeEmoji(0x1F611) << makeEmoji(0x1F636) << makeEmoji(0x1F60F);
    emojis << makeEmoji(0x1F612) << makeEmoji(0x1F644) << makeEmoji(0x1F62C) << makeEmoji(0x1F925);
    
    // More emotions
    emojis << makeEmoji(0x1F60C) << makeEmoji(0x1F614) << makeEmoji(0x1F62A) << makeEmoji(0x1F634);
    emojis << makeEmoji(0x1F631) << makeEmoji(0x1F628) << makeEmoji(0x1F622) << makeEmoji(0x1F62D);
    emojis << makeEmoji(0x1F624) << makeEmoji(0x1F620) << makeEmoji(0x1F621) << makeEmoji(0x1F608);
    emojis << makeEmoji(0x1F47F) << makeEmoji(0x1F480) << makeEmoji(0x1F4A9) << makeEmoji(0x1F921);
    
    // Hearts - add VS for basic heart
    emojis << makeEmoji(0x2764, true);  // ❤️ Red heart needs VS
    emojis << makeEmoji(0x1F9E1) << makeEmoji(0x1F49B) << makeEmoji(0x1F49A);
    emojis << makeEmoji(0x1F499) << makeEmoji(0x1F49C) << makeEmoji(0x1F5A4) << makeEmoji(0x1F90D);
    emojis << makeEmoji(0x1F495) << makeEmoji(0x1F496) << makeEmoji(0x1F497) << makeEmoji(0x1F493);
    emojis << makeEmoji(0x1F498) << makeEmoji(0x1F49D) << makeEmoji(0x1F49E) << makeEmoji(0x1F48B);
    
    // Gestures - add VS for basic ones
    emojis << makeEmoji(0x1F44D) << makeEmoji(0x1F44E) << makeEmoji(0x1F44A);
    emojis << makeEmoji(0x270A, true);  // ✊ needs VS
    emojis << makeEmoji(0x1F91B) << makeEmoji(0x1F91C) << makeEmoji(0x1F44F) << makeEmoji(0x1F64C);
    emojis << makeEmoji(0x1F450) << makeEmoji(0x1F64F) << makeEmoji(0x1F91D);
    emojis << makeEmoji(0x270C, true);  // ✌️ needs VS
    emojis << makeEmoji(0x1F91E) << makeEmoji(0x1F91F) << makeEmoji(0x1F918) << makeEmoji(0x1F44C);
    
    // Animals
    emojis << makeEmoji(0x1F436) << makeEmoji(0x1F431) << makeEmoji(0x1F42D) << makeEmoji(0x1F439);
    emojis << makeEmoji(0x1F430) << makeEmoji(0x1F98A) << makeEmoji(0x1F43B) << makeEmoji(0x1F43C);
    emojis << makeEmoji(0x1F428) << makeEmoji(0x1F42F) << makeEmoji(0x1F981) << makeEmoji(0x1F42E);
    emojis << makeEmoji(0x1F437) << makeEmoji(0x1F438) << makeEmoji(0x1F435) << makeEmoji(0x1F412);
    
    // Objects & Symbols - add VS for basic symbols
    emojis << makeEmoji(0x1F389) << makeEmoji(0x1F38A) << makeEmoji(0x1F388) << makeEmoji(0x1F381);
    emojis << makeEmoji(0x1F382) << makeEmoji(0x1F380) << makeEmoji(0x1F525);
    emojis << makeEmoji(0x2B50, true);  // ⭐ needs VS
    emojis << makeEmoji(0x1F31F);
    emojis << makeEmoji(0x2728, true);  // ✨ needs VS
    emojis << makeEmoji(0x1F4AB) << makeEmoji(0x1F4A5) << makeEmoji(0x1F4AF);
    emojis << makeEmoji(0x2705, true);  // ✅ needs VS
    emojis << makeEmoji(0x274C, true);  // ❌ needs VS
    emojis << makeEmoji(0x2753, true);  // ❓ needs VS
    
    int col = 0, row = 0;
    int colCount = 8;
    
    for (const QString &emoji : emojis) {
        QPushButton *emojiBtn = new QPushButton(emoji);
        emojiBtn->setFixedSize(36, 36);
        emojiBtn->setFont(QFont("Noto Color Emoji", 20));
        emojiBtn->setStyleSheet(
            "QPushButton { background-color: transparent; border: none; padding: 2px; }"
            "QPushButton:hover { background-color: #e3f2fd; border-radius: 6px; }"
        );
        connect(emojiBtn, &QPushButton::clicked, [this, emoji]() {
            onEmojiClicked(emoji);
        });
        emojiGrid->addWidget(emojiBtn, row, col);
        
        col++;
        if (col >= colCount) {
            col = 0;
            row++;
        }
    }
    
    scrollArea->setWidget(emojiContainer);
    pickerLayout->addWidget(scrollArea);
    
    m_emojiPicker->hide();
}

void ChatWidget::toggleEmojiPicker()
{
    if (m_emojiPicker->isVisible()) {
        m_emojiPicker->hide();
    } else {
        // Position the picker above the emoji button
        QPoint globalPos = m_emojiButton->mapToGlobal(QPoint(0, 0));
        m_emojiPicker->move(globalPos.x(), globalPos.y() - m_emojiPicker->sizeHint().height() - 10);
        m_emojiPicker->show();
    }
}

void ChatWidget::onEmojiClicked(const QString &emoji)
{
    m_messageInput->insert(emoji);
    m_emojiPicker->hide();
    m_messageInput->setFocus();
}

void ChatWidget::onFriendSelected(QListWidgetItem *item)
{
    m_groupsList->clearSelection();
    
    // Lấy username từ UserRole thay vì parse text
    QString username = item->data(Qt::UserRole).toString();
    bool isOnline = item->data(Qt::UserRole + 1).toBool();
    
    m_currentTarget = username;
    m_isChatWithGroup = false;
    m_groupMenuBtn->setVisible(false);  // Hide menu for 1-1 chat
    
    QString status = isOnline ? " (Online)" : " (Offline)";
    m_chatHeader->setText("Chat with: " + username + status);
    m_messageInput->setEnabled(true);
    m_sendButton->setEnabled(true);
    m_emojiButton->setEnabled(true);
    m_messageInput->setFocus();
    
    // Clear and load chat history from server
    m_chatListWidget->clear();
    m_currentOffset = 0;
    m_totalMessageCount = 0;
    m_loadMoreBtn->setVisible(false);
    
    // Show/hide seen status based on stored state
    if (m_messageSeenStatus.value(username, false)) {
        m_seenStatusLabel->setText("✓✓ Đã xem");
        m_seenStatusLabel->setVisible(true);
    } else {
        m_seenStatusLabel->setVisible(false);
    }
    
    m_client->sendChatHistoryPrivate(username, 0, 10);
    
    // Mark messages from this user as read
    m_client->sendMarkMessagesRead(username);
}

void ChatWidget::onGroupSelected(QListWidgetItem *item)
{
    m_friendsList->clearSelection();
    
    QString data = item->data(Qt::UserRole).toString();
    QStringList parts = data.split(":");
    if (parts.size() >= 2) {
        m_currentTarget = parts[0];  // group_id
        m_currentGroupName = parts[1];
        m_isChatWithGroup = true;
        m_groupMenuBtn->setVisible(true);  // Show menu for group chat
        
        m_chatHeader->setText("Group: " + m_currentGroupName);
        m_messageInput->setEnabled(true);
        m_sendButton->setEnabled(true);
        m_emojiButton->setEnabled(true);
        m_messageInput->setFocus();
        
        // Hide seen status for group chat
        m_seenStatusLabel->setVisible(false);
        
        // Clear and load chat history from server
        m_chatListWidget->clear();
        m_currentOffset = 0;
        m_totalMessageCount = 0;
        m_loadMoreBtn->setVisible(false);
        m_client->sendChatHistoryGroup(m_currentTarget, 0, 10);
    }
}

void ChatWidget::onSendClicked()
{
    QString message = m_messageInput->text().trimmed();
    if (message.isEmpty() || m_currentTarget.isEmpty()) return;
    
    if (m_isChatWithGroup) {
        m_client->sendGroupMessage(m_currentTarget, message);
    } else {
        m_client->sendPrivateMessage(m_currentTarget, message);
    }
    
    appendMessage(m_username, message, true);
    m_messageInput->clear();
}

void ChatWidget::appendMessage(const QString &sender, const QString &message, bool isMe)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm");
    
    // Create message bubble widget
    MessageBubble *bubble = new MessageBubble(sender, message, time, isMe);
    
    // Create list item and set size
    QListWidgetItem *item = new QListWidgetItem(m_chatListWidget);
    item->setSizeHint(bubble->sizeHint());
    
    m_chatListWidget->addItem(item);
    m_chatListWidget->setItemWidget(item, bubble);
    
    // Scroll to bottom
    m_chatListWidget->scrollToBottom();
    
    // Hide seen status and reset when sending a new message
    if (isMe && !m_isChatWithGroup) {
        m_messageSeenStatus[m_currentTarget] = false;
        m_seenStatusLabel->setVisible(false);
    }
}

void ChatWidget::onRefreshFriends()
{
    m_client->sendFriendList();
}

void ChatWidget::onRefreshGroups()
{
    m_client->sendGroupList();
}

void ChatWidget::onAddFriend()
{
    QString username = QInputDialog::getText(this, "Thêm bạn", "Nhập username:");
    if (!username.isEmpty()) {
        m_client->sendFriendRequest(username);
        QMessageBox::information(this, "Thành công", "Đã gửi lời mời kết bạn đến " + username);
    }
}

void ChatWidget::onCreateGroup()
{
    QString groupName = QInputDialog::getText(this, "Tạo nhóm", "Tên nhóm:");
    if (!groupName.isEmpty()) {
        m_client->sendCreateGroup(groupName);
    }
}

void ChatWidget::onJoinGroup()
{
    qDebug() << "onJoinGroup called - requesting all groups";
    m_client->sendAllGroups();
}

void ChatWidget::onAllGroupsReceived(const QList<QPair<QString, QString>> &groups)
{
    qDebug() << "onAllGroupsReceived called with" << groups.size() << "groups";
    if (groups.isEmpty()) {
        QMessageBox::information(this, "Tham gia nhóm", "Không có nhóm nào trong hệ thống");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Chọn nhóm để tham gia");
    dialog.setMinimumWidth(400);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *label = new QLabel("Chọn nhóm bạn muốn tham gia:");
    layout->addWidget(label);
    
    QListWidget *listWidget = new QListWidget;
    listWidget->setStyleSheet(
        "QListWidget { background-color: white; border: 1px solid #ddd; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #eee; }"
        "QListWidget::item:hover { background-color: #e3f2fd; }"
        "QListWidget::item:selected { background-color: #2196F3; color: white; }");
    
    for (const auto &group : groups) {
        QListWidgetItem *item = new QListWidgetItem(group.second);
        item->setData(Qt::UserRole, group.first);
        listWidget->addItem(item);
    }
    layout->addWidget(listWidget);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *joinBtn = new QPushButton("Tham gia");
    QPushButton *cancelBtn = new QPushButton("Hủy");
    buttonLayout->addStretch();
    buttonLayout->addWidget(joinBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);
    
    connect(joinBtn, &QPushButton::clicked, [&]() {
        QListWidgetItem *selected = listWidget->currentItem();
        if (selected) {
            QString groupId = selected->data(Qt::UserRole).toString();
            m_client->sendJoinGroup(groupId);
            QMessageBox::information(&dialog, "Thành công", "Đã gửi yêu cầu tham gia nhóm");
            m_client->sendGroupList();
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng chọn nhóm");
        }
    });
    
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(listWidget, &QListWidget::itemDoubleClicked, [&](QListWidgetItem *item) {
        QString groupId = item->data(Qt::UserRole).toString();
        m_client->sendJoinGroup(groupId);
        QMessageBox::information(&dialog, "Thành công", "Đã gửi yêu cầu tham gia nhóm");
        m_client->sendGroupList();
        dialog.accept();
    });
    
    dialog.exec();
}
void ChatWidget::onLeaveGroup()
{
    QListWidgetItem *item = m_groupsList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn nhóm cần rời");
        return;
    }
    
    QString data = item->data(Qt::UserRole).toString();
    QStringList parts = data.split(":");
    if (parts.size() >= 2) {
        if (QMessageBox::question(this, "Xác nhận", 
            "Bạn có chắc muốn rời khỏi nhóm " + parts[1] + "?") == QMessageBox::Yes) {
            m_client->sendLeaveGroup(parts[0]);
            m_client->sendGroupList();
        }
    }
}

void ChatWidget::onViewPendingRequests()
{
    m_client->sendPendingRequests();
}

void ChatWidget::onFriendListReceived(const QList<QPair<QString, bool>> &friends)
{
    m_friendsList->clear();
    for (const auto &f : friends) {
        QListWidgetItem *item = new QListWidgetItem(f.first);
        item->setData(Qt::UserRole, f.first);
        item->setData(Qt::UserRole + 1, f.second);  // Store online status
        
        if (f.second) {
            // Online - green dot icon
            item->setBackground(QBrush(QColor("#E8F5E9")));
            item->setForeground(QBrush(QColor("#2E7D32")));
            item->setText(QString::fromUtf8("\xE2\x97\x8F ") + f.first);  // Green filled circle
            item->setIcon(QIcon());
        } else {
            // Offline - gray dot
            item->setBackground(QBrush(QColor("#F5F5F5")));
            item->setForeground(QBrush(QColor("#757575")));
            item->setText(QString::fromUtf8("\xE2\x97\x8B ") + f.first);  // Empty circle
        }
        m_friendsList->addItem(item);
    }
}

void ChatWidget::onGroupListReceived(const QStringList &groups)
{
    m_groupsList->clear();
    for (const QString &g : groups) {
        QStringList parts = g.split(":");
        if (parts.size() >= 2) {
            QListWidgetItem *item = new QListWidgetItem("[Group] " + parts[1]);
            item->setData(Qt::UserRole, g);
            item->setBackground(QBrush(QColor("#E3F2FD")));
            item->setForeground(QBrush(QColor("#1565C0")));
            m_groupsList->addItem(item);
        }
    }
}

void ChatWidget::onPendingRequestsReceived(const QStringList &requests)
{
    if (requests.isEmpty()) {
        QMessageBox::information(this, "Lời mời kết bạn", "Không có lời mời nào");
        return;
    }
    
    for (const QString &from : requests) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
            "Lời mời kết bạn",
            from + " muốn kết bạn với bạn.\n\nChấp nhận?",
            QMessageBox::Yes | QMessageBox::No);
        
        m_client->sendFriendResponse(from, reply == QMessageBox::Yes);
    }
    
    m_client->sendFriendList();
}

void ChatWidget::onPrivateMessage(const QString &from, const QString &message)
{
    // If chatting with this person, show in chat and mark as read
    if (m_currentTarget == from && !m_isChatWithGroup) {
        appendMessage(from, message, false);
        // Mark message as read since we're viewing this chat
        m_client->sendMarkMessagesRead(from);
    } else {
        // Save to history and show notification
        QString key = from;
        QString html = m_chatHistory.value(key, "");
        QString time = QDateTime::currentDateTime().toString("hh:mm");
        html += QString(
            "<div style='text-align: left; margin: 5px;'>"
            "<span style='background-color: #e8f5e9; padding: 8px 12px; border-radius: 15px;'>"
            "<b style='color: #4CAF50;'>%1</b><br>%2"
            "<br><small style='color: #999;'>%3</small>"
            "</span></div>"
        ).arg(from, message.toHtmlEscaped(), time);
        m_chatHistory[key] = html;
        
        showNotification("Tin nhắn từ " + from, message);
    }
}

void ChatWidget::onGroupMessage(const QString &groupId, const QString &groupName,
                                const QString &from, const QString &message)
{
    if (from == m_username) return;  // Don't show own messages again
    
    if (m_currentTarget == groupId && m_isChatWithGroup) {
        appendMessage(from, message, false);
    } else {
        QString key = "group_" + groupId;
        QString html = m_chatHistory.value(key, "");
        QString time = QDateTime::currentDateTime().toString("hh:mm");
        html += QString(
            "<div style='text-align: left; margin: 5px;'>"
            "<span style='background-color: #e8f5e9; padding: 8px 12px; border-radius: 15px;'>"
            "<b style='color: #4CAF50;'>%1</b><br>%2"
            "<br><small style='color: #999;'>%3</small>"
            "</span></div>"
        ).arg(from, message.toHtmlEscaped(), time);
        m_chatHistory[key] = html;
        
        showNotification(groupName, from + ": " + message);
    }
}

void ChatWidget::onFriendRequest(const QString &from)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Lời mời kết bạn mới",
        from + " muốn kết bạn với bạn.\n\nChấp nhận?",
        QMessageBox::Yes | QMessageBox::No);
    
    m_client->sendFriendResponse(from, reply == QMessageBox::Yes);
    
    if (reply == QMessageBox::Yes) {
        m_client->sendFriendList();
    }
}

void ChatWidget::onFriendAccepted(const QString &username)
{
    QMessageBox::information(this, "Kết bạn thành công", 
        username + " đã chấp nhận lời mời kết bạn của bạn!");
    m_client->sendFriendList();
}

void ChatWidget::onFriendOnline(const QString &username)
{
    // Update friend status in list
    for (int i = 0; i < m_friendsList->count(); i++) {
        QListWidgetItem *item = m_friendsList->item(i);
        if (item->data(Qt::UserRole).toString() == username) {
            item->setText(QString::fromUtf8("\xE2\x97\x8F ") + username);  // Filled circle
            item->setBackground(QBrush(QColor("#E8F5E9")));
            item->setForeground(QBrush(QColor("#2E7D32")));
            item->setData(Qt::UserRole + 1, true);
            break;
        }
    }
}

void ChatWidget::onFriendOffline(const QString &username)
{
    for (int i = 0; i < m_friendsList->count(); i++) {
        QListWidgetItem *item = m_friendsList->item(i);
        if (item->data(Qt::UserRole).toString() == username) {
            item->setText(QString::fromUtf8("\xE2\x97\x8B ") + username);  // Empty circle
            item->setBackground(QBrush(QColor("#F5F5F5")));
            item->setForeground(QBrush(QColor("#757575")));
            item->setData(Qt::UserRole + 1, false);
            break;
        }
    }
}

void ChatWidget::showNotification(const QString &title, const QString &message)
{
    // Simple notification - could be improved with system tray
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(title);
    msgBox->setText(message);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->show();
    
    // Auto close after 3 seconds
    QTimer::singleShot(3000, msgBox, &QMessageBox::close);
}

void ChatWidget::onViewGroupMembers()
{
    if (!m_isChatWithGroup || m_currentTarget.isEmpty()) {
        return;
    }
    m_client->sendGroupMembers(m_currentTarget);
}

void ChatWidget::onLeaveCurrentGroup()
{
    if (!m_isChatWithGroup || m_currentTarget.isEmpty()) {
        return;
    }
    
    if (QMessageBox::question(this, "Leave Group", 
        "Are you sure you want to leave \"" + m_currentGroupName + "\"?") == QMessageBox::Yes) {
        m_client->sendLeaveGroup(m_currentTarget);
        
        // Reset chat area
        m_currentTarget.clear();
        m_currentGroupName.clear();
        m_isChatWithGroup = false;
        m_groupMenuBtn->setVisible(false);
        m_chatHeader->setText("Chon nguoi hoac nhom de chat");
        m_chatListWidget->clear();
        m_messageInput->setEnabled(false);
        m_sendButton->setEnabled(false);
        
        // Refresh group list
        m_client->sendGroupList();
    }
}

void ChatWidget::onGroupMembersReceived(const QString &groupId, const QString &groupName,
                                         const QList<QPair<QString, bool>> &members)
{
    Q_UNUSED(groupId);
    
    QString membersText = "<b>Members of \"" + groupName + "\":</b><br><br>";
    membersText += "<table style='width: 100%; border-collapse: collapse;'>";
    
    for (const auto &member : members) {
        QString statusColor = member.second ? "#4CAF50" : "#757575";
        QString statusText = member.second ? "Online" : "Offline";
        QString bgColor = member.second ? "#E8F5E9" : "#F5F5F5";
        
        membersText += QString(
            "<tr style='border-bottom: 1px solid #ddd;'>"
            "<td style='padding: 10px;'><b>%1</b></td>"
            "<td style='padding: 10px; text-align: right;'>"
            "<span style='background-color: %2; color: %3; padding: 4px 12px; border-radius: 10px;'>%4</span>"
            "</td></tr>"
        ).arg(member.first, bgColor, statusColor, statusText);
    }
    membersText += "</table>";
    membersText += "<br><small style='color: #666;'>Total: " + QString::number(members.size()) + " member(s)</small>";
    
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Group Members");
    msgBox->setTextFormat(Qt::RichText);
    msgBox->setText(membersText);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setMinimumWidth(400);
    msgBox->exec();
    delete msgBox;
}

void ChatWidget::onLogout()
{
    if (QMessageBox::question(this, "Logout", 
        "Are you sure you want to logout?") == QMessageBox::Yes) {
        emit logoutRequested();
    }
}

void ChatWidget::onChangePassword()
{
    // Create dialog for password change
    QDialog dialog(this);
    dialog.setWindowTitle("Change Password");
    dialog.setMinimumWidth(300);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Enter your passwords:");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(titleLabel);
    
    QLineEdit *oldPassEdit = new QLineEdit;
    oldPassEdit->setEchoMode(QLineEdit::Password);
    oldPassEdit->setPlaceholderText("Current Password");
    oldPassEdit->setMinimumHeight(35);
    oldPassEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
    oldPassEdit->setInputMethodHints(Qt::ImhNone);
    layout->addWidget(oldPassEdit);
    
    QLineEdit *newPassEdit = new QLineEdit;
    newPassEdit->setEchoMode(QLineEdit::Password);
    newPassEdit->setPlaceholderText("New Password");
    newPassEdit->setMinimumHeight(35);
    newPassEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
    newPassEdit->setInputMethodHints(Qt::ImhNone);
    layout->addWidget(newPassEdit);
    
    QLineEdit *confirmPassEdit = new QLineEdit;
    confirmPassEdit->setEchoMode(QLineEdit::Password);
    confirmPassEdit->setPlaceholderText("Confirm New Password");
    confirmPassEdit->setMinimumHeight(35);
    confirmPassEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
    confirmPassEdit->setInputMethodHints(Qt::ImhNone);
    layout->addWidget(confirmPassEdit);
    
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *cancelBtn = new QPushButton("Cancel");
    cancelBtn->setStyleSheet(
        "QPushButton { background-color: #9e9e9e; color: white; padding: 10px 20px; "
        "border-radius: 5px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #757575; }");
    QPushButton *changeBtn = new QPushButton("Change");
    changeBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 10px 20px; "
        "border-radius: 5px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #1976D2; }");
    
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(changeBtn);
    layout->addLayout(btnLayout);
    
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(changeBtn, &QPushButton::clicked, [&]() {
        QString oldPass = oldPassEdit->text();
        QString newPass = newPassEdit->text();
        QString confirmPass = confirmPassEdit->text();
        
        if (oldPass.isEmpty() || newPass.isEmpty() || confirmPass.isEmpty()) {
            QMessageBox::warning(&dialog, "Error", "Please fill in all fields");
            return;
        }
        
        if (newPass != confirmPass) {
            QMessageBox::warning(&dialog, "Error", "New passwords do not match");
            return;
        }
        
        if (newPass.length() < 4) {
            QMessageBox::warning(&dialog, "Error", "New password must be at least 4 characters");
            return;
        }
        
        m_client->sendChangePassword(oldPass, newPass);
        dialog.accept();
    });
    
    dialog.exec();
}

void ChatWidget::onChangePasswordResponse(bool success, const QString &message)
{
    if (success) {
        QMessageBox::information(this, "Success", "Password changed successfully!");
    } else {
        QMessageBox::warning(this, "Error", message.isEmpty() ? "Failed to change password" : message);
    }
}

void ChatWidget::loadChatHistory()
{
    if (m_currentTarget.isEmpty()) return;
    
    if (m_isChatWithGroup) {
        m_client->sendChatHistoryGroup(m_currentTarget, m_currentOffset, 10);
    } else {
        m_client->sendChatHistoryPrivate(m_currentTarget, m_currentOffset, 10);
    }
}

void ChatWidget::onLoadMoreMessages()
{
    if (m_currentTarget.isEmpty()) return;
    
    // Request next batch of messages
    if (m_isChatWithGroup) {
        m_client->sendChatHistoryGroup(m_currentTarget, m_currentOffset, 10);
    } else {
        m_client->sendChatHistoryPrivate(m_currentTarget, m_currentOffset, 10);
    }
}

void ChatWidget::onPrivateChatHistoryReceived(const QString &targetUsername, int totalCount, int offset,
                                              const QList<QMap<QString, QString>> &messages)
{
    // Make sure this is for the current chat
    if (targetUsername != m_currentTarget || m_isChatWithGroup) return;
    
    m_totalMessageCount = totalCount;
    m_currentOffset = offset + messages.size();
    
    // Messages come in DESC order (newest first), we need to reverse for display
    
    if (offset == 0) {
        // Initial load - clear and display (reverse order since DESC)
        m_chatListWidget->clear();
        for (int i = messages.size() - 1; i >= 0; i--) {
            const auto &msg = messages[i];
            QString sender = msg["from_username"];
            QString text = msg["message"];
            QString time = msg["sent_at"];
            bool isMe = (sender == m_username);
            
            // Format time
            QString displayTime = time;
            if (time.length() >= 16) {
                displayTime = time.mid(11, 5);  // Extract HH:MM
            }
            
            prependMessage(sender, text, displayTime, isMe);
        }
        m_chatListWidget->scrollToBottom();
    } else {
        // Loading older messages - insert at top
        for (int i = 0; i < messages.size(); i++) {
            const auto &msg = messages[i];
            QString sender = msg["from_username"];
            QString text = msg["message"];
            QString time = msg["sent_at"];
            bool isMe = (sender == m_username);
            
            QString displayTime = time;
            if (time.length() >= 16) {
                displayTime = time.mid(11, 5);
            }
            
            // Insert at top
            MessageBubble *bubble = new MessageBubble(sender, text, displayTime, isMe);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(bubble->sizeHint());
            m_chatListWidget->insertItem(i, item);
            m_chatListWidget->setItemWidget(item, bubble);
        }
    }
    
    // Show/hide load more button
    m_loadMoreBtn->setVisible(m_currentOffset < m_totalMessageCount);
}

void ChatWidget::onGroupChatHistoryReceived(const QString &groupId, const QString &groupName,
                                            int totalCount, int offset,
                                            const QList<QMap<QString, QString>> &messages)
{
    Q_UNUSED(groupName);
    
    // Make sure this is for the current chat
    if (groupId != m_currentTarget || !m_isChatWithGroup) return;
    
    m_totalMessageCount = totalCount;
    m_currentOffset = offset + messages.size();
    
    if (offset == 0) {
        // Initial load
        m_chatListWidget->clear();
        for (int i = messages.size() - 1; i >= 0; i--) {
            const auto &msg = messages[i];
            QString sender = msg["from_username"];
            QString text = msg["message"];
            QString time = msg["sent_at"];
            bool isMe = (sender == m_username);
            
            QString displayTime = time;
            if (time.length() >= 16) {
                displayTime = time.mid(11, 5);
            }
            
            prependMessage(sender, text, displayTime, isMe);
        }
        m_chatListWidget->scrollToBottom();
    } else {
        // Loading older messages - insert at top
        for (int i = 0; i < messages.size(); i++) {
            const auto &msg = messages[i];
            QString sender = msg["from_username"];
            QString text = msg["message"];
            QString time = msg["sent_at"];
            bool isMe = (sender == m_username);
            
            QString displayTime = time;
            if (time.length() >= 16) {
                displayTime = time.mid(11, 5);
            }
            
            MessageBubble *bubble = new MessageBubble(sender, text, displayTime, isMe);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(bubble->sizeHint());
            m_chatListWidget->insertItem(i, item);
            m_chatListWidget->setItemWidget(item, bubble);
        }
    }
    
    m_loadMoreBtn->setVisible(m_currentOffset < m_totalMessageCount);
}

void ChatWidget::prependMessage(const QString &sender, const QString &message, const QString &time, bool isMe)
{
    // Create message bubble widget
    MessageBubble *bubble = new MessageBubble(sender, message, time, isMe);
    
    // Create list item and set size
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(bubble->sizeHint());
    
    m_chatListWidget->addItem(item);
    m_chatListWidget->setItemWidget(item, bubble);
}

void ChatWidget::onMessagesReadNotification(const QString &readerUsername)
{
    // Someone read our messages - update the seen status
    m_messageSeenStatus[readerUsername] = true;
    
    // If we're currently chatting with this user, show "Đã xem"
    if (!m_isChatWithGroup && m_currentTarget == readerUsername) {
        m_seenStatusLabel->setText("✓✓ Đã xem");
        m_seenStatusLabel->setVisible(true);
    }
}
