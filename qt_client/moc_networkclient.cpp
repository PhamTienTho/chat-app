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
    QByteArrayData data[39];
    char stringdata0[537];
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
QT_MOC_LITERAL(34, 461, 11), // "onReadyRead"
QT_MOC_LITERAL(35, 473, 11), // "onConnected"
QT_MOC_LITERAL(36, 485, 14), // "onDisconnected"
QT_MOC_LITERAL(37, 500, 7), // "onError"
QT_MOC_LITERAL(38, 508, 28) // "QAbstractSocket::SocketError"

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
    "userLeftGroup\0onReadyRead\0onConnected\0"
    "onDisconnected\0onError\0"
    "QAbstractSocket::SocketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      19,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  129,    2, 0x06 /* Public */,
       3,    0,  130,    2, 0x06 /* Public */,
       4,    1,  131,    2, 0x06 /* Public */,
       6,    3,  134,    2, 0x06 /* Public */,
      10,    2,  141,    2, 0x06 /* Public */,
      11,    2,  146,    2, 0x06 /* Public */,
      12,    3,  151,    2, 0x06 /* Public */,
      14,    1,  158,    2, 0x06 /* Public */,
      16,    3,  161,    2, 0x06 /* Public */,
      20,    1,  168,    2, 0x06 /* Public */,
      22,    1,  171,    2, 0x06 /* Public */,
      24,    2,  174,    2, 0x06 /* Public */,
      26,    4,  179,    2, 0x06 /* Public */,
      27,    1,  188,    2, 0x06 /* Public */,
      28,    1,  191,    2, 0x06 /* Public */,
      30,    1,  194,    2, 0x06 /* Public */,
      31,    1,  197,    2, 0x06 /* Public */,
      32,    2,  200,    2, 0x06 /* Public */,
      33,    2,  205,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      34,    0,  210,    2, 0x08 /* Private */,
      35,    0,  211,    2, 0x08 /* Private */,
      36,    0,  212,    2, 0x08 /* Private */,
      37,    1,  213,    2, 0x08 /* Private */,

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

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 38,    5,

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
        case 19: _t->onReadyRead(); break;
        case 20: _t->onConnected(); break;
        case 21: _t->onDisconnected(); break;
        case 22: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 22:
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
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
