#-------------------------------------------------
#
# Project created by QtCreator 2017-09-30T23:36:12
#
#-------------------------------------------------

QT       -= gui
QT       += xml network

TARGET = SbSHandler
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug/
}else{
    DESTDIR = $$PWD/../bin/release/
}

DEFINES += SBSHANDLER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += sbs_handler.cpp \
    application_layer.cpp \
    network_layer.cpp \
    session_layer.cpp \
    server_proxy.cpp

HEADERS += sbs_handler.h\
        sbshandler_global.h \
    application_layer.h \
    data_queue.h \
    network_layer.h \
    session_layer.h \
    server_proxy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lXmlUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lXmlUtil
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lXmlUtil

INCLUDEPATH += $$PWD/../XmlUtil
DEPENDPATH += $$PWD/../XmlUtil

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lSockUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lSockUtil
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lSockUtil

INCLUDEPATH += $$PWD/../SockUtil
DEPENDPATH += $$PWD/../SockUtil
