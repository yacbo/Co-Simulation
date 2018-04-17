/****************************************************************************
** Meta object code from reading C++ file 'sbssoftui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sbssoftui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sbssoftui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SbsSoftUI_t {
    QByteArrayData data[12];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SbsSoftUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SbsSoftUI_t qt_meta_stringdata_SbsSoftUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SbsSoftUI"
QT_MOC_LITERAL(1, 10, 13), // "start_service"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "exit_service"
QT_MOC_LITERAL(4, 38, 12), // "rcv_reg_slot"
QT_MOC_LITERAL(5, 51, 8), // "dev_name"
QT_MOC_LITERAL(6, 60, 6), // "dev_ip"
QT_MOC_LITERAL(7, 67, 8), // "dev_port"
QT_MOC_LITERAL(8, 76, 6), // "dev_id"
QT_MOC_LITERAL(9, 83, 5), // "login"
QT_MOC_LITERAL(10, 89, 21), // "rcv_progress_log_slot"
QT_MOC_LITERAL(11, 111, 3) // "log"

    },
    "SbsSoftUI\0start_service\0\0exit_service\0"
    "rcv_reg_slot\0dev_name\0dev_ip\0dev_port\0"
    "dev_id\0login\0rcv_progress_log_slot\0"
    "log"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SbsSoftUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    5,   36,    2, 0x08 /* Private */,
      10,    1,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    5,    6,    7,    8,    9,
    QMetaType::Void, QMetaType::QString,   11,

       0        // eod
};

void SbsSoftUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SbsSoftUI *_t = static_cast<SbsSoftUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start_service(); break;
        case 1: _t->exit_service(); break;
        case 2: _t->rcv_reg_slot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 3: _t->rcv_progress_log_slot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SbsSoftUI::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SbsSoftUI.data,
      qt_meta_data_SbsSoftUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SbsSoftUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SbsSoftUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SbsSoftUI.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SbsSoftUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
