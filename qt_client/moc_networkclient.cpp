/****************************************************************************
** Meta object code from reading C++ file 'networkclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "networkclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkClient_t {
    QByteArrayData data[48];
    char stringdata0[701];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkClient_t qt_meta_stringdata_NetworkClient = {
    {
QT_MOC_LITERAL(0, 0, 13), // "NetworkClient"
QT_MOC_LITERAL(1, 14, 9), // "connected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "disconnected"
QT_MOC_LITERAL(4, 38, 15), // "connectionError"
QT_MOC_LITERAL(5, 54, 5), // "error"
QT_MOC_LITERAL(6, 60, 13), // "loginResponse"
QT_MOC_LITERAL(7, 74, 7), // "success"
QT_MOC_LITERAL(8, 82, 7), // "message"
QT_MOC_LITERAL(9, 90, 5), // "token"
QT_MOC_LITERAL(10, 96, 16), // "registerResponse"
QT_MOC_LITERAL(11, 113, 22), // "changePasswordResponse"
QT_MOC_LITERAL(12, 136, 12), // "groupCreated"
QT_MOC_LITERAL(13, 149, 7), // "groupId"
QT_MOC_LITERAL(14, 157, 17), // "groupListReceived"
QT_MOC_LITERAL(15, 175, 6), // "groups"
QT_MOC_LITERAL(16, 182, 20), // "groupMembersReceived"
QT_MOC_LITERAL(17, 203, 9), // "groupName"
QT_MOC_LITERAL(18, 213, 27), // "QList<QPair<QString,bool> >"
QT_MOC_LITERAL(19, 241, 7), // "members"
QT_MOC_LITERAL(20, 249, 18), // "friendListReceived"
QT_MOC_LITERAL(21, 268, 7), // "friends"
QT_MOC_LITERAL(22, 276, 23), // "pendingRequestsReceived"
QT_MOC_LITERAL(23, 300, 8), // "requests"
QT_MOC_LITERAL(24, 309, 22), // "privateMessageReceived"
QT_MOC_LITERAL(25, 332, 4), // "from"
QT_MOC_LITERAL(26, 337, 20), // "groupMessageReceived"
QT_MOC_LITERAL(27, 358, 21), // "friendRequestReceived"
QT_MOC_LITERAL(28, 380, 14), // "friendAccepted"
QT_MOC_LITERAL(29, 395, 8), // "username"
QT_MOC_LITERAL(30, 404, 12), // "friendOnline"
QT_MOC_LITERAL(31, 417, 13), // "friendOffline"
QT_MOC_LITERAL(32, 431, 15), // "userJoinedGroup"
QT_MOC_LITERAL(33, 447, 13), // "userLeftGroup"
QT_MOC_LITERAL(34, 461, 26), // "privateChatHistoryReceived"
QT_MOC_LITERAL(35, 488, 14), // "targetUsername"
QT_MOC_LITERAL(36, 503, 10), // "totalCount"
QT_MOC_LITERAL(37, 514, 6), // "offset"
QT_MOC_LITERAL(38, 521, 29), // "QList<QMap<QString,QString> >"
QT_MOC_LITERAL(39, 551, 8), // "messages"
QT_MOC_LITERAL(40, 560, 24), // "groupChatHistoryReceived"
QT_MOC_LITERAL(41, 585, 24), // "messagesReadNotification"
QT_MOC_LITERAL(42, 610, 14), // "readerUsername"
QT_MOC_LITERAL(43, 625, 11), // "onReadyRead"
QT_MOC_LITERAL(44, 637, 11), // "onConnected"
QT_MOC_LITERAL(45, 649, 14), // "onDisconnected"
QT_MOC_LITERAL(46, 664, 7), // "onError"
QT_MOC_LITERAL(47, 672, 28) // "QAbstractSocket::SocketError"

    },
    "NetworkClient\0connected\0\0disconnected\0"
    "connectionError\0error\0loginResponse\0"
    "success\0message\0token\0registerResponse\0"
    "changePasswordResponse\0groupCreated\0"
    "groupId\0groupListReceived\0groups\0"
    "groupMembersReceived\0groupName\0"
    "QList<QPair<QString,bool> >\0members\0"
    "friendListReceived\0friends\0"
    "pendingRequestsReceived\0requests\0"
    "privateMessageReceived\0from\0"
    "groupMessageReceived\0friendRequestReceived\0"
    "friendAccepted\0username\0friendOnline\0"
    "friendOffline\0userJoinedGroup\0"
    "userLeftGroup\0privateChatHistoryReceived\0"
    "targetUsername\0totalCount\0offset\0"
    "QList<QMap<QString,QString> >\0messages\0"
    "groupChatHistoryReceived\0"
    "messagesReadNotification\0readerUsername\0"
    "onReadyRead\0onConnected\0onDisconnected\0"
    "onError\0QAbstractSocket::SocketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      22,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  144,    2, 0x06 /* Public */,
       3,    0,  145,    2, 0x06 /* Public */,
       4,    1,  146,    2, 0x06 /* Public */,
       6,    3,  149,    2, 0x06 /* Public */,
      10,    2,  156,    2, 0x06 /* Public */,
      11,    2,  161,    2, 0x06 /* Public */,
      12,    3,  166,    2, 0x06 /* Public */,
      14,    1,  173,    2, 0x06 /* Public */,
      16,    3,  176,    2, 0x06 /* Public */,
      20,    1,  183,    2, 0x06 /* Public */,
      22,    1,  186,    2, 0x06 /* Public */,
      24,    2,  189,    2, 0x06 /* Public */,
      26,    4,  194,    2, 0x06 /* Public */,
      27,    1,  203,    2, 0x06 /* Public */,
      28,    1,  206,    2, 0x06 /* Public */,
      30,    1,  209,    2, 0x06 /* Public */,
      31,    1,  212,    2, 0x06 /* Public */,
      32,    2,  215,    2, 0x06 /* Public */,
      33,    2,  220,    2, 0x06 /* Public */,
      34,    4,  225,    2, 0x06 /* Public */,
      40,    5,  234,    2, 0x06 /* Public */,
      41,    1,  245,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      43,    0,  248,    2, 0x08 /* Private */,
      44,    0,  249,    2, 0x08 /* Private */,
      45,    0,  250,    2, 0x08 /* Private */,
      46,    1,  251,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QString,    7,    8,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QString,    7,    8,   13,
    QMetaType::Void, QMetaType::QStringList,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 18,   13,   17,   19,
    QMetaType::Void, 0x80000000 | 18,   21,
    QMetaType::Void, QMetaType::QStringList,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   25,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   13,   17,   25,    8,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 38,   35,   36,   37,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 38,   13,   17,   36,   37,   39,
    QMetaType::Void, QMetaType::QString,   42,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 47,    5,

       0        // eod
};

void NetworkClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->loginResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->registerResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->changePasswordResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->groupCreated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 7: _t->groupListReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 8: _t->groupMembersReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[3]))); break;
        case 9: _t->friendListReceived((*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[1]))); break;
        case 10: _t->pendingRequestsReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 11: _t->privateMessageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->groupMessageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 13: _t->friendRequestReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->friendAccepted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->friendOnline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->friendOffline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->userJoinedGroup((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 18: _t->userLeftGroup((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 19: _t->privateChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[4]))); break;
        case 20: _t->groupChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[5]))); break;
        case 21: _t->messagesReadNotification((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->onReadyRead(); break;
        case 23: _t->onConnected(); break;
        case 24: _t->onDisconnected(); break;
        case 25: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 25:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(bool , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::loginResponse)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::registerResponse)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::changePasswordResponse)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(bool , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupCreated)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupListReceived)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , const QList<QPair<QString,bool>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupMembersReceived)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QList<QPair<QString,bool>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendListReceived)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::pendingRequestsReceived)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::privateMessageReceived)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupMessageReceived)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendRequestReceived)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendAccepted)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendOnline)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendOffline)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::userJoinedGroup)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::userLeftGroup)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , int , int , const QList<QMap<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::privateChatHistoryReceived)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , int , int , const QList<QMap<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupChatHistoryReceived)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::messagesReadNotification)) {
                *result = 21;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkClient.data,
    qt_meta_data_NetworkClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void NetworkClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkClient::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkClient::loginResponse(bool _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkClient::registerResponse(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkClient::changePasswordResponse(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkClient::groupCreated(bool _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkClient::groupListReceived(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void NetworkClient::groupMembersReceived(const QString & _t1, const QString & _t2, const QList<QPair<QString,bool>> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void NetworkClient::friendListReceived(const QList<QPair<QString,bool>> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NetworkClient::pendingRequestsReceived(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NetworkClient::privateMessageReceived(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void NetworkClient::groupMessageReceived(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void NetworkClient::friendRequestReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void NetworkClient::friendAccepted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void NetworkClient::friendOnline(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void NetworkClient::friendOffline(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void NetworkClient::userJoinedGroup(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void NetworkClient::userLeftGroup(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void NetworkClient::privateChatHistoryReceived(const QString & _t1, int _t2, int _t3, const QList<QMap<QString,QString>> & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void NetworkClient::groupChatHistoryReceived(const QString & _t1, const QString & _t2, int _t3, int _t4, const QList<QMap<QString,QString>> & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void NetworkClient::messagesReadNotification(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
