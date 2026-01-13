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
    QByteArrayData data[67];
    char stringdata0[989];
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
QT_MOC_LITERAL(16, 182, 17), // "allGroupsReceived"
QT_MOC_LITERAL(17, 200, 30), // "QList<QPair<QString,QString> >"
QT_MOC_LITERAL(18, 231, 19), // "allGroupsListSimple"
QT_MOC_LITERAL(19, 251, 5), // "items"
QT_MOC_LITERAL(20, 257, 20), // "groupMembersReceived"
QT_MOC_LITERAL(21, 278, 9), // "groupName"
QT_MOC_LITERAL(22, 288, 27), // "QList<QPair<QString,bool> >"
QT_MOC_LITERAL(23, 316, 7), // "members"
QT_MOC_LITERAL(24, 324, 18), // "friendListReceived"
QT_MOC_LITERAL(25, 343, 7), // "friends"
QT_MOC_LITERAL(26, 351, 16), // "allUsersReceived"
QT_MOC_LITERAL(27, 368, 5), // "users"
QT_MOC_LITERAL(28, 374, 23), // "pendingRequestsReceived"
QT_MOC_LITERAL(29, 398, 8), // "requests"
QT_MOC_LITERAL(30, 407, 22), // "privateMessageReceived"
QT_MOC_LITERAL(31, 430, 4), // "from"
QT_MOC_LITERAL(32, 435, 9), // "messageId"
QT_MOC_LITERAL(33, 445, 18), // "privateMessageSent"
QT_MOC_LITERAL(34, 464, 14), // "targetUsername"
QT_MOC_LITERAL(35, 479, 20), // "groupMessageReceived"
QT_MOC_LITERAL(36, 500, 16), // "groupMessageSent"
QT_MOC_LITERAL(37, 517, 21), // "friendRequestReceived"
QT_MOC_LITERAL(38, 539, 14), // "friendAccepted"
QT_MOC_LITERAL(39, 554, 8), // "username"
QT_MOC_LITERAL(40, 563, 12), // "friendOnline"
QT_MOC_LITERAL(41, 576, 13), // "friendOffline"
QT_MOC_LITERAL(42, 590, 15), // "userJoinedGroup"
QT_MOC_LITERAL(43, 606, 13), // "userLeftGroup"
QT_MOC_LITERAL(44, 620, 19), // "groupInviteResponse"
QT_MOC_LITERAL(45, 640, 26), // "privateChatHistoryReceived"
QT_MOC_LITERAL(46, 667, 10), // "totalCount"
QT_MOC_LITERAL(47, 678, 6), // "offset"
QT_MOC_LITERAL(48, 685, 29), // "QList<QMap<QString,QString> >"
QT_MOC_LITERAL(49, 715, 8), // "messages"
QT_MOC_LITERAL(50, 724, 24), // "groupChatHistoryReceived"
QT_MOC_LITERAL(51, 749, 24), // "messagesReadNotification"
QT_MOC_LITERAL(52, 774, 14), // "readerUsername"
QT_MOC_LITERAL(53, 789, 20), // "fileDownloadReceived"
QT_MOC_LITERAL(54, 810, 8), // "fileName"
QT_MOC_LITERAL(55, 819, 8), // "fileData"
QT_MOC_LITERAL(56, 828, 8), // "fileSize"
QT_MOC_LITERAL(57, 837, 21), // "deleteMessageResponse"
QT_MOC_LITERAL(58, 859, 14), // "messageDeleted"
QT_MOC_LITERAL(59, 874, 8), // "chatType"
QT_MOC_LITERAL(60, 883, 21), // "searchResultsReceived"
QT_MOC_LITERAL(61, 905, 7), // "results"
QT_MOC_LITERAL(62, 913, 11), // "onReadyRead"
QT_MOC_LITERAL(63, 925, 11), // "onConnected"
QT_MOC_LITERAL(64, 937, 14), // "onDisconnected"
QT_MOC_LITERAL(65, 952, 7), // "onError"
QT_MOC_LITERAL(66, 960, 28) // "QAbstractSocket::SocketError"

    },
    "NetworkClient\0connected\0\0disconnected\0"
    "connectionError\0error\0loginResponse\0"
    "success\0message\0token\0registerResponse\0"
    "changePasswordResponse\0groupCreated\0"
    "groupId\0groupListReceived\0groups\0"
    "allGroupsReceived\0QList<QPair<QString,QString> >\0"
    "allGroupsListSimple\0items\0"
    "groupMembersReceived\0groupName\0"
    "QList<QPair<QString,bool> >\0members\0"
    "friendListReceived\0friends\0allUsersReceived\0"
    "users\0pendingRequestsReceived\0requests\0"
    "privateMessageReceived\0from\0messageId\0"
    "privateMessageSent\0targetUsername\0"
    "groupMessageReceived\0groupMessageSent\0"
    "friendRequestReceived\0friendAccepted\0"
    "username\0friendOnline\0friendOffline\0"
    "userJoinedGroup\0userLeftGroup\0"
    "groupInviteResponse\0privateChatHistoryReceived\0"
    "totalCount\0offset\0QList<QMap<QString,QString> >\0"
    "messages\0groupChatHistoryReceived\0"
    "messagesReadNotification\0readerUsername\0"
    "fileDownloadReceived\0fileName\0fileData\0"
    "fileSize\0deleteMessageResponse\0"
    "messageDeleted\0chatType\0searchResultsReceived\0"
    "results\0onReadyRead\0onConnected\0"
    "onDisconnected\0onError\0"
    "QAbstractSocket::SocketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      32,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  194,    2, 0x06 /* Public */,
       3,    0,  195,    2, 0x06 /* Public */,
       4,    1,  196,    2, 0x06 /* Public */,
       6,    3,  199,    2, 0x06 /* Public */,
      10,    2,  206,    2, 0x06 /* Public */,
      11,    2,  211,    2, 0x06 /* Public */,
      12,    3,  216,    2, 0x06 /* Public */,
      14,    1,  223,    2, 0x06 /* Public */,
      16,    1,  226,    2, 0x06 /* Public */,
      18,    1,  229,    2, 0x06 /* Public */,
      20,    3,  232,    2, 0x06 /* Public */,
      24,    1,  239,    2, 0x06 /* Public */,
      26,    1,  242,    2, 0x06 /* Public */,
      28,    1,  245,    2, 0x06 /* Public */,
      30,    3,  248,    2, 0x06 /* Public */,
      33,    2,  255,    2, 0x06 /* Public */,
      35,    5,  260,    2, 0x06 /* Public */,
      36,    2,  271,    2, 0x06 /* Public */,
      37,    1,  276,    2, 0x06 /* Public */,
      38,    1,  279,    2, 0x06 /* Public */,
      40,    1,  282,    2, 0x06 /* Public */,
      41,    1,  285,    2, 0x06 /* Public */,
      42,    2,  288,    2, 0x06 /* Public */,
      43,    2,  293,    2, 0x06 /* Public */,
      44,    2,  298,    2, 0x06 /* Public */,
      45,    4,  303,    2, 0x06 /* Public */,
      50,    5,  312,    2, 0x06 /* Public */,
      51,    1,  323,    2, 0x06 /* Public */,
      53,    3,  326,    2, 0x06 /* Public */,
      57,    3,  333,    2, 0x06 /* Public */,
      58,    3,  340,    2, 0x06 /* Public */,
      60,    1,  347,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      62,    0,  350,    2, 0x08 /* Private */,
      63,    0,  351,    2, 0x08 /* Private */,
      64,    0,  352,    2, 0x08 /* Private */,
      65,    1,  353,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QString,    7,    8,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QString,    7,    8,   13,
    QMetaType::Void, QMetaType::QStringList,   15,
    QMetaType::Void, 0x80000000 | 17,   15,
    QMetaType::Void, QMetaType::QStringList,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 22,   13,   21,   23,
    QMetaType::Void, 0x80000000 | 22,   25,
    QMetaType::Void, 0x80000000 | 17,   27,
    QMetaType::Void, QMetaType::QStringList,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,   31,    8,   32,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   32,   34,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   13,   21,   31,    8,   32,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   32,   13,
    QMetaType::Void, QMetaType::QString,   31,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   39,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 48,   34,   46,   47,   49,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, 0x80000000 | 48,   13,   21,   46,   47,   49,
    QMetaType::Void, QMetaType::QString,   52,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray, QMetaType::LongLong,   54,   55,   56,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::Int,    7,    8,   32,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,   32,   59,   13,
    QMetaType::Void, 0x80000000 | 48,   61,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 66,    5,

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
        case 8: _t->allGroupsReceived((*reinterpret_cast< const QList<QPair<QString,QString> >(*)>(_a[1]))); break;
        case 9: _t->allGroupsListSimple((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 10: _t->groupMembersReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[3]))); break;
        case 11: _t->friendListReceived((*reinterpret_cast< const QList<QPair<QString,bool> >(*)>(_a[1]))); break;
        case 12: _t->allUsersReceived((*reinterpret_cast< const QList<QPair<QString,QString> >(*)>(_a[1]))); break;
        case 13: _t->pendingRequestsReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 14: _t->privateMessageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 15: _t->privateMessageSent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 16: _t->groupMessageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 17: _t->groupMessageSent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 18: _t->friendRequestReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->friendAccepted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->friendOnline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->friendOffline((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->userJoinedGroup((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 23: _t->userLeftGroup((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 24: _t->groupInviteResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 25: _t->privateChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[4]))); break;
        case 26: _t->groupChatHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[5]))); break;
        case 27: _t->messagesReadNotification((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 28: _t->fileDownloadReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 29: _t->deleteMessageResponse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 30: _t->messageDeleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 31: _t->searchResultsReceived((*reinterpret_cast< const QList<QMap<QString,QString> >(*)>(_a[1]))); break;
        case 32: _t->onReadyRead(); break;
        case 33: _t->onConnected(); break;
        case 34: _t->onDisconnected(); break;
        case 35: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 35:
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
            using _t = void (NetworkClient::*)(const QList<QPair<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::allGroupsReceived)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::allGroupsListSimple)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , const QList<QPair<QString,bool>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupMembersReceived)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QList<QPair<QString,bool>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendListReceived)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QList<QPair<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::allUsersReceived)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::pendingRequestsReceived)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::privateMessageReceived)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::privateMessageSent)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , const QString & , const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupMessageReceived)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupMessageSent)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendRequestReceived)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendAccepted)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendOnline)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::friendOffline)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::userJoinedGroup)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::userLeftGroup)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupInviteResponse)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , int , int , const QList<QMap<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::privateChatHistoryReceived)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QString & , int , int , const QList<QMap<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::groupChatHistoryReceived)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::messagesReadNotification)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QString & , const QByteArray & , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::fileDownloadReceived)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(bool , const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::deleteMessageResponse)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::messageDeleted)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (NetworkClient::*)(const QList<QMap<QString,QString>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClient::searchResultsReceived)) {
                *result = 31;
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
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
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
void NetworkClient::allGroupsReceived(const QList<QPair<QString,QString>> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void NetworkClient::allGroupsListSimple(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NetworkClient::groupMembersReceived(const QString & _t1, const QString & _t2, const QList<QPair<QString,bool>> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NetworkClient::friendListReceived(const QList<QPair<QString,bool>> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void NetworkClient::allUsersReceived(const QList<QPair<QString,QString>> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void NetworkClient::pendingRequestsReceived(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void NetworkClient::privateMessageReceived(const QString & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void NetworkClient::privateMessageSent(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void NetworkClient::groupMessageReceived(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, int _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void NetworkClient::groupMessageSent(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void NetworkClient::friendRequestReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void NetworkClient::friendAccepted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void NetworkClient::friendOnline(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void NetworkClient::friendOffline(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void NetworkClient::userJoinedGroup(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void NetworkClient::userLeftGroup(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void NetworkClient::groupInviteResponse(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void NetworkClient::privateChatHistoryReceived(const QString & _t1, int _t2, int _t3, const QList<QMap<QString,QString>> & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void NetworkClient::groupChatHistoryReceived(const QString & _t1, const QString & _t2, int _t3, int _t4, const QList<QMap<QString,QString>> & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void NetworkClient::messagesReadNotification(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void NetworkClient::fileDownloadReceived(const QString & _t1, const QByteArray & _t2, qint64 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void NetworkClient::deleteMessageResponse(bool _t1, const QString & _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void NetworkClient::messageDeleted(int _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void NetworkClient::searchResultsReceived(const QList<QMap<QString,QString>> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
