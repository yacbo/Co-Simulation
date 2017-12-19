#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T20:23:54
#
#-------------------------------------------------

QT       -= gui

TARGET = PowerHandler
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug/
}else{
    DESTDIR = $$PWD/../bin/release/
}

DEFINES += POWERHANDLER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += powerhandler.cpp \
       Utils.cpp

HEADERS += powerhandler.h\
        powerhandler_global.h \
        Utils.hpp \

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
win32: LIBS += -L$$PWD/./ -ldigapi
win32: LIBS += -L$$PWD/./ -ldigapivalue

