#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T12:10:07
#
#-------------------------------------------------

QT       += core gui xml
RC_FILE = myapp.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SbsSoftUI
TEMPLATE = app

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug/
}else{
    DESTDIR = $$PWD/../bin/release/
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        sbssoftui.cpp

HEADERS  += sbssoftui.h

FORMS    += sbssoftui.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lSbSHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lSbSHandler
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lSbSHandler

INCLUDEPATH += $$PWD/../SbSHandler
DEPENDPATH += $$PWD/../SbSHandler

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lXmlUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lXmlUtil
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lXmlUtil

INCLUDEPATH += $$PWD/../XmlUtil
DEPENDPATH += $$PWD/../XmlUtil

RESOURCES += \
    sbs.qrc

DISTFILES +=

