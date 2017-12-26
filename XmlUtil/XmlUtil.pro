#-------------------------------------------------
#
# Project created by QtCreator 2017-09-30T23:28:34
#
#-------------------------------------------------

QT       += xml network
QT       -= gui

TARGET = XmlUtil
TEMPLATE = lib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug/
}else{
    DESTDIR = $$PWD/../bin/release/
}

DEFINES += XMLUTIL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += xml_util.cpp \
    xml_message.cpp \
    xml_frame_util.cpp \
    log_util.cpp

HEADERS += xml_util.h\
        xmlutil_global.h \
    xml_message.h \
    comm_table.h \
    comm_type_def.h \
    log_util.h \
    pg_rtui_def.h \
    power_data_def.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
