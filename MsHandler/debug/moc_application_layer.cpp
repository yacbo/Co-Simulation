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
    QByteArrayData data[30];
    char stringdata0[357];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_application_layer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_application_layer_t qt_meta_stringdata_application_layer = {
    {
QT_MOC_LITERAL(0, 0, 17), // "application_layer"
QT_MOC_LITERAL(1, 18, 16), // "snd_upper_signal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 12), // "ApplMessage*"
QT_MOC_LITERAL(4, 49, 3), // "msg"
QT_MOC_LITERAL(5, 53, 16), // "snd_lower_signal"
QT_MOC_LITERAL(6, 70, 13), // "QDomDocument*"
QT_MOC_LITERAL(7, 84, 3), // "doc"
QT_MOC_LITERAL(8, 88, 6), // "dst_ip"
QT_MOC_LITERAL(9, 95, 4), // "port"
QT_MOC_LITERAL(10, 100, 25), // "snd_lower_register_signal"
QT_MOC_LITERAL(11, 126, 5), // "ss_id"
QT_MOC_LITERAL(12, 132, 6), // "sbs_ip"
QT_MOC_LITERAL(13, 139, 8), // "sbs_port"
QT_MOC_LITERAL(14, 148, 10), // "proto_type"
QT_MOC_LITERAL(15, 159, 8), // "dev_port"
QT_MOC_LITERAL(16, 168, 27), // "snd_lower_unregister_signal"
QT_MOC_LITERAL(17, 196, 25), // "ack_comm_cfg_param_signal"
QT_MOC_LITERAL(18, 222, 13), // "PG_RTUI_Base*"
QT_MOC_LITERAL(19, 236, 4), // "data"
QT_MOC_LITERAL(20, 241, 12), // "login_signal"
QT_MOC_LITERAL(21, 254, 8), // "dev_name"
QT_MOC_LITERAL(22, 263, 6), // "dev_ip"
QT_MOC_LITERAL(23, 270, 5), // "login"
QT_MOC_LITERAL(24, 276, 19), // "progress_log_signal"
QT_MOC_LITERAL(25, 296, 3), // "log"
QT_MOC_LITERAL(26, 300, 15), // "rcv_lower_slots"
QT_MOC_LITERAL(27, 316, 23), // "rcv_lower_type_id_slots"
QT_MOC_LITERAL(28, 340, 7), // "IntMap*"
QT_MOC_LITERAL(29, 348, 8) // "type_ids"

    },
    "application_layer\0snd_upper_signal\0\0"
    "ApplMessage*\0msg\0snd_lower_signal\0"
    "QDomDocument*\0doc\0dst_ip\0port\0"
    "snd_lower_register_signal\0ss_id\0sbs_ip\0"
    "sbs_port\0proto_type\0dev_port\0"
    "snd_lower_unregister_signal\0"
    "ack_comm_cfg_param_signal\0PG_RTUI_Base*\0"
    "data\0login_signal\0dev_name\0dev_ip\0"
    "login\0progress_log_signal\0log\0"
    "rcv_lower_slots\0rcv_lower_type_id_slots\0"
    "IntMap*\0type_ids"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_application_layer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       5,    3,   67,    2, 0x06 /* Public */,
      10,    5,   74,    2, 0x06 /* Public */,
      16,    3,   85,    2, 0x06 /* Public */,
      17,    1,   92,    2, 0x06 /* Public */,
      20,    4,   95,    2, 0x06 /* Public */,
      20,    3,  104,    2, 0x26 /* Public | MethodCloned */,
      24,    1,  111,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      26,    1,  114,    2, 0x08 /* Private */,
      27,    1,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QString, QMetaType::UShort,    7,    8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::UShort,   11,   12,    9,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Bool,   21,   22,   15,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,   21,   22,   15,
    QMetaType::Void, QMetaType::QString,   25,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 28,   29,

       0        // eod
};

void application_layer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        application_layer *_t = static_cast<application_layer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->snd_upper_signal((*reinterpret_cast< ApplMessage*(*)>(_a[1]))); break;
        case 1: _t->snd_lower_signal((*reinterpret_cast< QDomDocument*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 2: _t->snd_lower_register_signal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: _t->snd_lower_unregister_signal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 4: _t->ack_comm_cfg_param_signal((*reinterpret_cast< PG_RTUI_Base*(*)>(_a[1]))); break;
        case 5: _t->login_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 6: _t->login_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->progress_log_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->rcv_lower_slots((*reinterpret_cast< ApplMessage*(*)>(_a[1]))); break;
        case 9: _t->rcv_lower_type_id_slots((*reinterpret_cast< IntMap*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (application_layer::*_t)(ApplMessage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::snd_upper_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(QDomDocument * , QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::snd_lower_signal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(int , QString , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::snd_lower_register_signal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(int , QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::snd_lower_unregister_signal)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(PG_RTUI_Base * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::ack_comm_cfg_param_signal)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(QString , QString , int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::login_signal)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (application_layer::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&application_layer::progress_log_signal)) {
                *result = 7;
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void application_layer::snd_upper_signal(ApplMessage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void application_layer::snd_lower_signal(QDomDocument * _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void application_layer::snd_lower_register_signal(int _t1, QString _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void application_layer::snd_lower_unregister_signal(int _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void application_layer::ack_comm_cfg_param_signal(PG_RTUI_Base * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void application_layer::login_signal(QString _t1, QString _t2, int _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 7
void application_layer::progress_log_signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
