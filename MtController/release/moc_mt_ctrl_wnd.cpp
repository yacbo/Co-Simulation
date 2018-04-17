/****************************************************************************
** Meta object code from reading C++ file 'mt_ctrl_wnd.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mt_ctrl_wnd.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mt_ctrl_wnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[304];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 17), // "config_Init_event"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 22), // "start_simulation_event"
QT_MOC_LITERAL(4, 53, 21), // "stop_simulation_event"
QT_MOC_LITERAL(5, 75, 15), // "register_device"
QT_MOC_LITERAL(6, 91, 17), // "unregister_device"
QT_MOC_LITERAL(7, 109, 15), // "elec_info_event"
QT_MOC_LITERAL(8, 125, 19), // "comm_net_note_event"
QT_MOC_LITERAL(9, 145, 14), // "elec_app_event"
QT_MOC_LITERAL(10, 160, 17), // "menu_action_event"
QT_MOC_LITERAL(11, 178, 18), // "login_singal_slots"
QT_MOC_LITERAL(12, 197, 8), // "dev_name"
QT_MOC_LITERAL(13, 206, 6), // "dev_ip"
QT_MOC_LITERAL(14, 213, 8), // "uint16_t"
QT_MOC_LITERAL(15, 222, 8), // "dev_port"
QT_MOC_LITERAL(16, 231, 5), // "login"
QT_MOC_LITERAL(17, 237, 18), // "progress_log_slots"
QT_MOC_LITERAL(18, 256, 3), // "log"
QT_MOC_LITERAL(19, 260, 24), // "ack_comm_cfg_param_slots"
QT_MOC_LITERAL(20, 285, 13), // "PG_RTUI_Base*"
QT_MOC_LITERAL(21, 299, 4) // "data"

    },
    "MainWindow\0config_Init_event\0\0"
    "start_simulation_event\0stop_simulation_event\0"
    "register_device\0unregister_device\0"
    "elec_info_event\0comm_net_note_event\0"
    "elec_app_event\0menu_action_event\0"
    "login_singal_slots\0dev_name\0dev_ip\0"
    "uint16_t\0dev_port\0login\0progress_log_slots\0"
    "log\0ack_comm_cfg_param_slots\0PG_RTUI_Base*\0"
    "data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    0,   81,    2, 0x08 /* Private */,
      10,    0,   82,    2, 0x08 /* Private */,
      11,    4,   83,    2, 0x08 /* Private */,
      17,    1,   92,    2, 0x08 /* Private */,
      19,    1,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 14, QMetaType::Bool,   12,   13,   15,   16,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->config_Init_event(); break;
        case 1: _t->start_simulation_event(); break;
        case 2: _t->stop_simulation_event(); break;
        case 3: _t->register_device(); break;
        case 4: _t->unregister_device(); break;
        case 5: _t->elec_info_event(); break;
        case 6: _t->comm_net_note_event(); break;
        case 7: _t->elec_app_event(); break;
        case 8: _t->menu_action_event(); break;
        case 9: _t->login_singal_slots((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 10: _t->progress_log_slots((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->ack_comm_cfg_param_slots((*reinterpret_cast< PG_RTUI_Base*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
