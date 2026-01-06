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
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MessageBubble_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MessageBubble_t qt_meta_stringdata_MessageBubble = {
    {
QT_MOC_LITERAL(0, 0, 13) // "MessageBubble"

    },
    "MessageBubble"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MessageBubble[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MessageBubble::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
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
    return _id;
}
struct qt_meta_stringdata_ChatWidget_t {
    QByteArrayData data[51];
    char stringdata0[776];
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
QT_MOC_LITERAL(20, 270, 18), // "onViewGroupMembers"
QT_MOC_LITERAL(21, 289, 19), // "onLeaveCurrentGroup"
QT_MOC_LITERAL(22, 309, 22), // "onGroupMembersReceived"
QT_MOC_LITERAL(23, 332, 7), // "groupId"
QT_MOC_LITERAL(24, 340, 9), // "groupName"
QT_MOC_LITERAL(25, 350, 27), // "QList<QPair<QString,bool> >"
QT_MOC_LITERAL(26, 378, 7), // "members"
QT_MOC_LITERAL(27, 386, 20), // "onFriendListReceived"
QT_MOC_LITERAL(28, 407, 7), // "friends"
QT_MOC_LITERAL(29, 415, 19), // "onGroupListReceived"
QT_MOC_LITERAL(30, 435, 6), // "groups"
QT_MOC_LITERAL(31, 442, 25), // "onPendingRequestsReceived"
QT_MOC_LITERAL(32, 468, 8), // "requests"
QT_MOC_LITERAL(33, 477, 16), // "onPrivateMessage"
QT_MOC_LITERAL(34, 494, 4), // "from"
QT_MOC_LITERAL(35, 499, 14), // "onGroupMessage"
QT_MOC_LITERAL(36, 514, 15), // "onFriendRequest"
QT_MOC_LITERAL(37, 530, 16), // "onFriendAccepted"
QT_MOC_LITERAL(38, 547, 8), // "username"
QT_MOC_LITERAL(39, 556, 14), // "onFriendOnline"
QT_MOC_LITERAL(40, 571, 15), // "onFriendOffline"
QT_MOC_LITERAL(41, 587, 18), // "onLoadMoreMessages"
QT_MOC_LITERAL(42, 606, 28), // "onPrivateChatHistoryReceived"
QT_MOC_LITERAL(43, 635, 14), // "targetUsername"
QT_MOC_LITERAL(44, 650, 10), // "totalCount"
QT_MOC_LITERAL(45, 661, 6), // "offset"
QT_MOC_LITERAL(46, 668, 29), // "QList<QMap<QString,QString> >"
QT_MOC_LITERAL(47, 698, 8), // "messages"
QT_MOC_LITERAL(48, 707, 26), // "onGroupChatHistoryReceived"
QT_MOC_LITERAL(49, 734, 26), // "onMessagesReadNotification"
QT_MOC_LITERAL(50, 761, 14) // "readerUsername"

    },
    "ChatWidget\0logoutRequested\0\0onSendClicked\0"
    "onLogout\0onChangePassword\0"
    "onChangePasswordResponse\0success\0"
    "message\0onFriendSelected\0QListWidgetItem*\0"
    "item\0onGroupSelected\0onRefreshFriends\0"
    "onRefreshGroups\0onAddFriend\0onCreateGroup\0"
    "onJoinGroup\0onLeaveGroup\0onViewPendingRequests\0"
    "onViewGroupMembers\0onLeaveCurrentGroup\0"
    "onGroupMembersReceived\0groupId\0groupName\0"
    "QList<QPair<QString,bool> >\0members\0"
    "onFriendListReceived\0friends\0"
    "onGroupListReceived\0groups\0"
    "onPendingRequestsReceived\0requests\0"
    "onPrivateMessage\0from\0onGroupMessage\0"
    "onFriendRequest\0onFriendAccepted\0"
    "username\0onFriendOnline\0onFriendOffline\0"
    "onLoadMoreMessages\0onPrivateChatHistoryReceived\0"
    "targetUsername\0totalCount\0offset\0"
    "QList<QMap<QString,QString> >\0messages\0"
    "onGroupChatHistoryReceived\0"
    "onMessagesReadNotification\0readerUsername"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  164,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  165,    2, 0x08 /* Private */,
       4,    0,  166,    2, 0x08 /* Private */,
       5,    0,  167,    2, 0x08 /* Private */,
       6,    2,  168,    2, 0x08 /* Private */,
       9,    1,  173,    2, 0x08 /* Private */,
      12,    1,  176,    2, 0x08 /* Private */,
      13,    0,  179,    2, 0x08 /* Private */,
      14,    0,  180,    2, 0x08 /* Private */,
      15,    0,  181,    2, 0x08 /* Private */,
      16,    0,  182,    2, 0x08 /* Private */,
      17,    0,  183,    2, 0x08 /* Private */,
      18,    0,  184,    2, 0x08 /* Private */,
      19,    0,  185,    2, 0x08 /* Private */,
      20,    0,  186,    2, 0x08 /* Private */,
      21,    0,  187,    2, 0x08 /* Private */,
      22,    3,  188,    2, 0x08 /* Private */,
      27,    1,  195,    2, 0x08 /* Private */,
      29,    1,  198,    2, 0x08 /* Private */,
      31,    1,  201,    2, 0x08 /* Private */,
      33,    2,  204,    2, 0x08 /* Private */,
      35,    4,  209,    2, 0x08 /* Private */,
      36,    1,  218,    2, 0x08 /* Private */,
      37,    1,  221,    2, 0x08 /* Private */,
      39,    1,  224,    2, 0x08 /* Private */,
      40,    1,  227,    2, 0x08 /* Private */,
      41,    0,  230,    2, 0x08 /* Private */,
      42,    4,  231,    2, 0x08 /* Private */,
      48,    5,  240,    2, 0x08 /* Private */,
      49,    1,  251,    2, 0x08 /* Private */,

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
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 25,   23,   24,   26,
    QMetaType::Void, 0x80000000 | 25,   28,
    QMetaType::Void, QMetaType::QStringList,   30,
    QMetaType::Void, QMetaType::QStringList,   32,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   34,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   23,   24,   34,    8,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 46,   43,   44,   45,   47,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 46,   23,   24,   44,   45,   47,
    QMetaType::Void, QMetaType::QString,   50,

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
        case 14: _t->onViewGroupMembers(); break;
        case 15: _t->onLeaveCurrentGroup(); break;
        case 16: _t->onGroupMembersReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[3]))); break;
        case 17: _t->onFriendListReceived((*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[1]))); break;
        case 18: _t->onGroupListReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 19: _t->onPendingRequestsReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 20: _t->onPrivateMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->onGroupMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 22: _t->onFriendRequest((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->onFriendAccepted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->onFriendOnline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: _t->onFriendOffline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->onLoadMoreMessages(); break;
        case 27: _t->onPrivateChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[4]))); break;
        case 28: _t->onGroupChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[5]))); break;
        case 29: _t->onMessagesReadNotification((*reinterpret_cast< const QString(*)>(_a[1]))); break;
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
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 30;
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
