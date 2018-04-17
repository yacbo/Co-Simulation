/****************************************************************************
** Meta object code from reading C++ file 'midsoftui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../midsoftui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midsoftui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MidSoftUI_t {
    QByteArrayData data[17];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MidSoftUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MidSoftUI_t qt_meta_stringdata_MidSoftUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MidSoftUI"
QT_MOC_LITERAL(1, 10, 15), // "register_device"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 17), // "unregister_device"
QT_MOC_LITERAL(4, 45, 19), // "combox_select_event"
QT_MOC_LITERAL(5, 65, 1), // "a"
QT_MOC_LITERAL(6, 67, 18), // "login_singal_slots"
QT_MOC_LITERAL(7, 86, 8), // "dev_name"
QT_MOC_LITERAL(8, 95, 6), // "dev_ip"
QT_MOC_LITERAL(9, 102, 8), // "uint16_t"
QT_MOC_LITERAL(10, 111, 8), // "dev_port"
QT_MOC_LITERAL(11, 120, 5), // "login"
QT_MOC_LITERAL(12, 126, 18), // "progress_log_slots"
QT_MOC_LITERAL(13, 145, 3), // "log"
QT_MOC_LITERAL(14, 149, 10), // "paintEvent"
QT_MOC_LITERAL(15, 160, 12), // "QPaintEvent*"
QT_MOC_LITERAL(16, 173, 6) // "pEvent"

    },
    "MidSoftUI\0register_device\0\0unregister_device\0"
    "combox_select_event\0a\0login_singal_slots\0"
    "dev_name\0dev_ip\0uint16_t\0dev_port\0"
    "login\0progress_log_slots\0log\0paintEvent\0"
    "QPaintEvent*\0pEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MidSoftUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       6,    4,   49,    2, 0x08 /* Private */,
      12,    1,   58,    2, 0x08 /* Private */,
      14,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 9, QMetaType::Bool,    7,    8,   10,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void MidSoftUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MidSoftUI *_t = static_cast<MidSoftUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->register_device(); break;
        case 1: _t->unregister_device(); break;
        case 2: _t->combox_select_event((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->login_singal_slots((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 4: _t->progress_log_slots((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MidSoftUI::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MidSoftUI.data,
      qt_meta_data_MidSoftUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MidSoftUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MidSoftUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MidSoftUI.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MidSoftUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
