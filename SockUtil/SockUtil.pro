#-------------------------------------------------
#
# Project created by QtCreator 2017-09-30T23:31:40
#
#-------------------------------------------------

QT       += network concurrent

QT       -= gui

TARGET = SockUtil
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug/
}else{
    DESTDIR = $$PWD/../bin/release/
}

DEFINES += SOCKUTIL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += sock_util.cpp \
    tcp_handler.cpp \
    udp_handler.cpp \
    net_handler.cpp \
    sockethandler.cpp

HEADERS += sock_util.h\
        sockutil_global.h \
    udp_handler.h \
    tcp_handler.h \
    net_handler.h \
    sockethandler.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../XmlUtil
win32: LIBS += $$PWD/./ws2_32.lib

