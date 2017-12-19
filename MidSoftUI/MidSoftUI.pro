#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T12:29:50
#
#-------------------------------------------------

QT       += core gui xml network
RC_FILE = myapp.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidSoftUI
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
        midsoftui.cpp \
    comm_config_dlg.cpp

HEADERS  += midsoftui.h \
    comm_config_dlg.h

FORMS    += midsoftui.ui \
    comm_config_dlg.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lMsHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lMsHandler
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lMsHandler

INCLUDEPATH += $$PWD/../MsHandler
DEPENDPATH += $$PWD/../MsHandler

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lXmlUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lXmlUtil
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lXmlUtil

INCLUDEPATH += $$PWD/../XmlUtil
DEPENDPATH += $$PWD/../XmlUtil

RESOURCES += \
    minsoft.qrc
