/****************************************************************************
** Meta object code from reading C++ file 'application_layer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../application_layer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'application_layer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_application_layer_t {
    QByteArrayData data[22];
    char stringdata0[236];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_application_layer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_application_layer_t qt_meta_stringdata_application_layer = {
    {
QT_MOC_LITERAL(0, 0, 17), // "application_layer"
QT_MOC_LITERAL(1, 18, 16), // "snd_lower_signal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 13), // "QDomDocument*"
QT_MOC_LITERAL(4, 50, 3), // "doc"
QT_MOC_LITERAL(5, 54, 6), // "dst_ip"
QT_MOC_LITERAL(6, 61, 4), // "port"
QT_MOC_LITERAL(7, 66, 15), // "ui_login_signal"
QT_MOC_LITERAL(8, 82, 8), // "dev_name"
QT_MOC_LITERAL(9, 91, 6), // "dev_ip"
QT_MOC_LITERAL(10, 98, 8), // "dev_port"
QT_MOC_LITERAL(11, 107, 6), // "dev_id"
QT_MOC_LITERAL(12, 114, 5), // "login"
QT_MOC_LITERAL(13, 120, 19), // "progress_log_signal"
QT_MOC_LITERAL(14, 140, 3), // "log"
QT_MOC_LITERAL(15, 144, 15), // "rcv_lower_slots"
QT_MOC_LITERAL(16, 160, 12), // "ApplMessage*"
QT_MOC_LITERAL(17, 173, 3), // "msg"
QT_MOC_LITERAL(18, 177, 23), // "rcv_lower_type_id_slots"
QT_MOC_LITERAL(19, 201, 4), // "type"
QT_MOC_LITERAL(20, 206, 2), // "id"
QT_MOC_LITERAL(21, 209, 26) // "check_sim_time_event_slots"

    },
    "application_layer\0snd_lower_signal\0\0"
    "QDomDocument*\0doc\0dst_ip\0port\0"
    "ui_login_signal\0dev_name\0dev_ip\0"
    "dev_port\0dev_id\0login\0progress_log_signal\0"
    "log\0rcv_lower_slots\0ApplMessage*\0msg\0"
    "rcv_lower_type_id_slots\0type\0id\0"
    "check_sim_time_event_slots"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_application_layer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x06 /* Public */,
       7,    5,   51,    2, 0x06 /* Public */,
      13,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    1,   65,    2, 0x08 /* Private */,
      18,    2,   68,    2, 0x08 /* Private */,
      21,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::UShort,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   19,   20,
    QMetaType::Void,

       0        // eod
};

void application_layer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        application_layer *_t = static_cast<application_layer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->snd_lower_signal((*reinterpret_cast< QDomDocument*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 1: _t->ui_login_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 2: _t->progress_log_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->rcv_lower_slots((*reinterpret_cast< ApplMessage*(*)>(_a[1]))); break;
        case 4: _t->rcv_lower_type_id_slots((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->check_sim_time_event_slots(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (application_layer::*_t)(QDomDocument * , QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::snd_lower_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(QString , QString , int , int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::ui_login_signal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::progress_log_signal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject application_layer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_application_layer.data,
      qt_meta_data_application_layer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *application_layer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *application_layer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_application_layer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int application_layer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void application_layer::snd_lower_signal(QDomDocument * _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void application_layer::ui_login_signal(QString _t1, QString _t2, int _t3, int _t4, bool _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void application_layer::progress_log_signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
