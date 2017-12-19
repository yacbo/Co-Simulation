#-------------------------------------------------
#
# Project created by QtCreator 2017-09-30T23:41:53
#
#-------------------------------------------------

QT  +=  core xml gui network
RC_FILE = myapp.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MtController
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
        mt_ctrl_wnd.cpp \
    comm_param_settings.cpp \
    arch_param_settings.cpp

HEADERS  += mt_ctrl_wnd.h \
    comm_param_settings.h \
    arch_param_settings.h

FORMS    += mt_ctrl_wnd.ui \
    comm_param_settings.ui \
    arch_param_settings.ui

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

RESOURCES += \
    resourece.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lMsHandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lMsHandler
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lMsHandler

INCLUDEPATH += $$PWD/../MsHandler
DEPENDPATH += $$PWD/../MsHandler
