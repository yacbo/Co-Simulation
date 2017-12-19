QT += core xml
QT -= gui

CONFIG += c++11
QT       += xml xmlpatterns
TARGET = SimTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug/
}else{
    DESTDIR = $$PWD/../bin/release/
}

SOURCES += main.cpp \
    sock_test.cpp \
    xmltest.cpp \
    mshandler_test.cpp \
    power_handler.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lXmlUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lXmlUtil
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lXmlUtil

INCLUDEPATH += $$PWD/../XmlUtil
DEPENDPATH += $$PWD/../XmlUtil

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SockUtil/release/ -lSockUtil
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SockUtil/debug/ -lSockUtil
#else:unix:!macx: LIBS += -L$$OUT_PWD/../SockUtil/ -lSockUtil

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lSockUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lSockUtil
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lSockUtil

INCLUDEPATH += $$PWD/../SockUtil
DEPENDPATH += $$PWD/../SockUtil

HEADERS += \
    header_test.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lMsHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lMsHandler
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lMsHandler

INCLUDEPATH += $$PWD/../MsHandler
DEPENDPATH += $$PWD/../MsHandler

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lPowerHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lPowerHandler

INCLUDEPATH += $$PWD/../PowerHandler
DEPENDPATH += $$PWD/../PowerHandler
