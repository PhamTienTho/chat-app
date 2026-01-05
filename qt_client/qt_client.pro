QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = chat_client_gui
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    chatwidget.cpp \
    networkclient.cpp

HEADERS += \
    mainwindow.h \
    loginwindow.h \
    chatwidget.h \
    networkclient.h \
    ../common/protocol.h \
    ../common/json_helper.h

INCLUDEPATH += ../common

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
