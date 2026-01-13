/****************************************************************************
** Meta object code from reading C++ file 'chatwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "chatwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MessageBubble_t {
    QByteArrayData data[4];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MessageBubble_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MessageBubble_t qt_meta_stringdata_MessageBubble = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MessageBubble"
QT_MOC_LITERAL(1, 14, 15), // "deleteRequested"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 9) // "messageId"

    },
    "MessageBubble\0deleteRequested\0\0messageId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MessageBubble[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void MessageBubble::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MessageBubble *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->deleteRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MessageBubble::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageBubble::deleteRequested)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MessageBubble::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MessageBubble.data,
    qt_meta_data_MessageBubble,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MessageBubble::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MessageBubble::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MessageBubble.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MessageBubble::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MessageBubble::deleteRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ChatWidget_t {
    QByteArrayData data[75];
    char stringdata0[1163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatWidget_t qt_meta_stringdata_ChatWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ChatWidget"
QT_MOC_LITERAL(1, 11, 15), // "logoutRequested"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 13), // "onSendClicked"
QT_MOC_LITERAL(4, 42, 8), // "onLogout"
QT_MOC_LITERAL(5, 51, 16), // "onChangePassword"
QT_MOC_LITERAL(6, 68, 24), // "onChangePasswordResponse"
QT_MOC_LITERAL(7, 93, 7), // "success"
QT_MOC_LITERAL(8, 101, 7), // "message"
QT_MOC_LITERAL(9, 109, 16), // "onFriendSelected"
QT_MOC_LITERAL(10, 126, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 143, 4), // "item"
QT_MOC_LITERAL(12, 148, 15), // "onGroupSelected"
QT_MOC_LITERAL(13, 164, 16), // "onRefreshFriends"
QT_MOC_LITERAL(14, 181, 15), // "onRefreshGroups"
QT_MOC_LITERAL(15, 197, 11), // "onAddFriend"
QT_MOC_LITERAL(16, 209, 13), // "onCreateGroup"
QT_MOC_LITERAL(17, 223, 11), // "onJoinGroup"
QT_MOC_LITERAL(18, 235, 12), // "onLeaveGroup"
QT_MOC_LITERAL(19, 248, 21), // "onViewPendingRequests"
QT_MOC_LITERAL(20, 270, 10), // "onUnfriend"
QT_MOC_LITERAL(21, 281, 12), // "onAttachFile"
QT_MOC_LITERAL(22, 294, 12), // "downloadFile"
QT_MOC_LITERAL(23, 307, 8), // "fileName"
QT_MOC_LITERAL(24, 316, 18), // "onViewGroupMembers"
QT_MOC_LITERAL(25, 335, 19), // "onLeaveCurrentGroup"
QT_MOC_LITERAL(26, 355, 21), // "onInviteMemberToGroup"
QT_MOC_LITERAL(27, 377, 21), // "onGroupInviteResponse"
QT_MOC_LITERAL(28, 399, 22), // "onGroupMembersReceived"
QT_MOC_LITERAL(29, 422, 7), // "groupId"
QT_MOC_LITERAL(30, 430, 9), // "groupName"
QT_MOC_LITERAL(31, 440, 27), // "QList<QPair<QString,bool> >"
QT_MOC_LITERAL(32, 468, 7), // "members"
QT_MOC_LITERAL(33, 476, 20), // "onFriendListReceived"
QT_MOC_LITERAL(34, 497, 7), // "friends"
QT_MOC_LITERAL(35, 505, 19), // "onGroupListReceived"
QT_MOC_LITERAL(36, 525, 6), // "groups"
QT_MOC_LITERAL(37, 532, 19), // "onAllGroupsReceived"
QT_MOC_LITERAL(38, 552, 30), // "QList<QPair<QString,QString> >"
QT_MOC_LITERAL(39, 583, 25), // "onPendingRequestsReceived"
QT_MOC_LITERAL(40, 609, 8), // "requests"
QT_MOC_LITERAL(41, 618, 16), // "onPrivateMessage"
QT_MOC_LITERAL(42, 635, 4), // "from"
QT_MOC_LITERAL(43, 640, 9), // "messageId"
QT_MOC_LITERAL(44, 650, 14), // "onGroupMessage"
QT_MOC_LITERAL(45, 665, 15), // "onFriendRequest"
QT_MOC_LITERAL(46, 681, 16), // "onFriendAccepted"
QT_MOC_LITERAL(47, 698, 8), // "username"
QT_MOC_LITERAL(48, 707, 14), // "onFriendOnline"
QT_MOC_LITERAL(49, 722, 15), // "onFriendOffline"
QT_MOC_LITERAL(50, 738, 18), // "onLoadMoreMessages"
QT_MOC_LITERAL(51, 757, 28), // "onPrivateChatHistoryReceived"
QT_MOC_LITERAL(52, 786, 14), // "targetUsername"
QT_MOC_LITERAL(53, 801, 10), // "totalCount"
QT_MOC_LITERAL(54, 812, 6), // "offset"
QT_MOC_LITERAL(55, 819, 29), // "QList<QMap<QString,QString> >"
QT_MOC_LITERAL(56, 849, 8), // "messages"
QT_MOC_LITERAL(57, 858, 26), // "onGroupChatHistoryReceived"
QT_MOC_LITERAL(58, 885, 26), // "onMessagesReadNotification"
QT_MOC_LITERAL(59, 912, 14), // "readerUsername"
QT_MOC_LITERAL(60, 927, 22), // "onFileDownloadReceived"
QT_MOC_LITERAL(61, 950, 8), // "fileData"
QT_MOC_LITERAL(62, 959, 8), // "fileSize"
QT_MOC_LITERAL(63, 968, 24), // "onDeleteMessageRequested"
QT_MOC_LITERAL(64, 993, 23), // "onDeleteMessageResponse"
QT_MOC_LITERAL(65, 1017, 16), // "onMessageDeleted"
QT_MOC_LITERAL(66, 1034, 8), // "chatType"
QT_MOC_LITERAL(67, 1043, 20), // "onPrivateMessageSent"
QT_MOC_LITERAL(68, 1064, 18), // "onGroupMessageSent"
QT_MOC_LITERAL(69, 1083, 14), // "onSearchToggle"
QT_MOC_LITERAL(70, 1098, 19), // "onSearchTextChanged"
QT_MOC_LITERAL(71, 1118, 4), // "text"
QT_MOC_LITERAL(72, 1123, 12), // "onSearchNext"
QT_MOC_LITERAL(73, 1136, 12), // "onSearchPrev"
QT_MOC_LITERAL(74, 1149, 13) // "onSearchClose"

    },
    "ChatWidget\0logoutRequested\0\0onSendClicked\0"
    "onLogout\0onChangePassword\0"
    "onChangePasswordResponse\0success\0"
    "message\0onFriendSelected\0QListWidgetItem*\0"
    "item\0onGroupSelected\0onRefreshFriends\0"
    "onRefreshGroups\0onAddFriend\0onCreateGroup\0"
    "onJoinGroup\0onLeaveGroup\0onViewPendingRequests\0"
    "onUnfriend\0onAttachFile\0downloadFile\0"
    "fileName\0onViewGroupMembers\0"
    "onLeaveCurrentGroup\0onInviteMemberToGroup\0"
    "onGroupInviteResponse\0onGroupMembersReceived\0"
    "groupId\0groupName\0QList<QPair<QString,bool> >\0"
    "members\0onFriendListReceived\0friends\0"
    "onGroupListReceived\0groups\0"
    "onAllGroupsReceived\0QList<QPair<QString,QString> >\0"
    "onPendingRequestsReceived\0requests\0"
    "onPrivateMessage\0from\0messageId\0"
    "onGroupMessage\0onFriendRequest\0"
    "onFriendAccepted\0username\0onFriendOnline\0"
    "onFriendOffline\0onLoadMoreMessages\0"
    "onPrivateChatHistoryReceived\0"
    "targetUsername\0totalCount\0offset\0"
    "QList<QMap<QString,QString> >\0messages\0"
    "onGroupChatHistoryReceived\0"
    "onMessagesReadNotification\0readerUsername\0"
    "onFileDownloadReceived\0fileData\0"
    "fileSize\0onDeleteMessageRequested\0"
    "onDeleteMessageResponse\0onMessageDeleted\0"
    "chatType\0onPrivateMessageSent\0"
    "onGroupMessageSent\0onSearchToggle\0"
    "onSearchTextChanged\0text\0onSearchNext\0"
    "onSearchPrev\0onSearchClose"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      47,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  249,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  250,    2, 0x08 /* Private */,
       4,    0,  251,    2, 0x08 /* Private */,
       5,    0,  252,    2, 0x08 /* Private */,
       6,    2,  253,    2, 0x08 /* Private */,
       9,    1,  258,    2, 0x08 /* Private */,
      12,    1,  261,    2, 0x08 /* Private */,
      13,    0,  264,    2, 0x08 /* Private */,
      14,    0,  265,    2, 0x08 /* Private */,
      15,    0,  266,    2, 0x08 /* Private */,
      16,    0,  267,    2, 0x08 /* Private */,
      17,    0,  268,    2, 0x08 /* Private */,
      18,    0,  269,    2, 0x08 /* Private */,
      19,    0,  270,    2, 0x08 /* Private */,
      20,    0,  271,    2, 0x08 /* Private */,
      21,    0,  272,    2, 0x08 /* Private */,
      22,    1,  273,    2, 0x0a /* Public */,
      24,    0,  276,    2, 0x08 /* Private */,
      25,    0,  277,    2, 0x08 /* Private */,
      26,    0,  278,    2, 0x08 /* Private */,
      27,    2,  279,    2, 0x08 /* Private */,
      28,    3,  284,    2, 0x08 /* Private */,
      33,    1,  291,    2, 0x08 /* Private */,
      35,    1,  294,    2, 0x08 /* Private */,
      37,    1,  297,    2, 0x08 /* Private */,
      39,    1,  300,    2, 0x08 /* Private */,
      41,    3,  303,    2, 0x08 /* Private */,
      44,    5,  310,    2, 0x08 /* Private */,
      45,    1,  321,    2, 0x08 /* Private */,
      46,    1,  324,    2, 0x08 /* Private */,
      48,    1,  327,    2, 0x08 /* Private */,
      49,    1,  330,    2, 0x08 /* Private */,
      50,    0,  333,    2, 0x08 /* Private */,
      51,    4,  334,    2, 0x08 /* Private */,
      57,    5,  343,    2, 0x08 /* Private */,
      58,    1,  354,    2, 0x08 /* Private */,
      60,    3,  357,    2, 0x08 /* Private */,
      63,    1,  364,    2, 0x08 /* Private */,
      64,    3,  367,    2, 0x08 /* Private */,
      65,    3,  374,    2, 0x08 /* Private */,
      67,    2,  381,    2, 0x08 /* Private */,
      68,    2,  386,    2, 0x08 /* Private */,
      69,    0,  391,    2, 0x08 /* Private */,
      70,    1,  392,    2, 0x08 /* Private */,
      72,    0,  395,    2, 0x08 /* Private */,
      73,    0,  396,    2, 0x08 /* Private */,
      74,    0,  397,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 31,   29,   30,   32,
    QMetaType::Void, 0x80000000 | 31,   34,
    QMetaType::Void, QMetaType::QStringList,   36,
    QMetaType::Void, 0x80000000 | 38,   36,
    QMetaType::Void, QMetaType::QStringList,   40,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,   42,    8,   43,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   29,   30,   42,    8,   43,
    QMetaType::Void, QMetaType::QString,   42,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 55,   52,   53,   54,   56,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 55,   29,   30,   53,   54,   56,
    QMetaType::Void, QMetaType::QString,   59,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray, QMetaType::LongLong,   23,   61,   62,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::Int,    7,    8,   43,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,   43,   66,   29,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   43,   52,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   43,   29,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   71,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChatWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->logoutRequested(); break;
        case 1: _t->onSendClicked(); break;
        case 2: _t->onLogout(); break;
        case 3: _t->onChangePassword(); break;
        case 4: _t->onChangePasswordResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onFriendSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->onGroupSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->onRefreshFriends(); break;
        case 8: _t->onRefreshGroups(); break;
        case 9: _t->onAddFriend(); break;
        case 10: _t->onCreateGroup(); break;
        case 11: _t->onJoinGroup(); break;
        case 12: _t->onLeaveGroup(); break;
        case 13: _t->onViewPendingRequests(); break;
        case 14: _t->onUnfriend(); break;
        case 15: _t->onAttachFile(); break;
        case 16: _t->downloadFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->onViewGroupMembers(); break;
        case 18: _t->onLeaveCurrentGroup(); break;
        case 19: _t->onInviteMemberToGroup(); break;
        case 20: _t->onGroupInviteResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->onGroupMembersReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[3]))); break;
        case 22: _t->onFriendListReceived((*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[1]))); break;
        case 23: _t->onGroupListReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 24: _t->onAllGroupsReceived((*reinterpret_cast< const QList<QPair<QString,QString> >(*)>(_a[1]))); break;
        case 25: _t->onPendingRequestsReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 26: _t->onPrivateMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 27: _t->onGroupMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 28: _t->onFriendRequest((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 29: _t->onFriendAccepted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 30: _t->onFriendOnline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 31: _t->onFriendOffline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->onLoadMoreMessages(); break;
        case 33: _t->onPrivateChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[4]))); break;
        case 34: _t->onGroupChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[5]))); break;
        case 35: _t->onMessagesReadNotification((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 36: _t->onFileDownloadReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 37: _t->onDeleteMessageRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->onDeleteMessageResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 39: _t->onMessageDeleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 40: _t->onPrivateMessageSent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 41: _t->onGroupMessageSent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 42: _t->onSearchToggle(); break;
        case 43: _t->onSearchTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 44: _t->onSearchNext(); break;
        case 45: _t->onSearchPrev(); break;
        case 46: _t->onSearchClose(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChatWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatWidget::logoutRequested)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChatWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ChatWidget.data,
    qt_meta_data_ChatWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChatWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChatWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 47)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 47;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 47)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 47;
    }
    return _id;
}

// SIGNAL 0
void ChatWidget::logoutRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
