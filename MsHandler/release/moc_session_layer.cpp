/****************************************************************************
** Meta object code from reading C++ file 'session_layer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../session_layer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'session_layer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_session_layer_t {
    QByteArrayData data[29];
    char stringdata0[362];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_session_layer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_session_layer_t qt_meta_stringdata_session_layer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "session_layer"
QT_MOC_LITERAL(1, 14, 16), // "snd_upper_signal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "ApplMessage*"
QT_MOC_LITERAL(4, 45, 3), // "msg"
QT_MOC_LITERAL(5, 49, 24), // "snd_upper_type_id_signal"
QT_MOC_LITERAL(6, 74, 7), // "IntMap*"
QT_MOC_LITERAL(7, 82, 8), // "type_ids"
QT_MOC_LITERAL(8, 91, 16), // "snd_lower_signal"
QT_MOC_LITERAL(9, 108, 13), // "QDomDocument*"
QT_MOC_LITERAL(10, 122, 3), // "doc"
QT_MOC_LITERAL(11, 126, 6), // "dst_ip"
QT_MOC_LITERAL(12, 133, 8), // "dst_port"
QT_MOC_LITERAL(13, 142, 25), // "snd_lower_register_signal"
QT_MOC_LITERAL(14, 168, 6), // "sbs_ip"
QT_MOC_LITERAL(15, 175, 8), // "sbs_port"
QT_MOC_LITERAL(16, 184, 10), // "proto_type"
QT_MOC_LITERAL(17, 195, 8), // "dev_port"
QT_MOC_LITERAL(18, 204, 27), // "snd_lower_unregister_signal"
QT_MOC_LITERAL(19, 232, 19), // "progress_log_signal"
QT_MOC_LITERAL(20, 252, 3), // "log"
QT_MOC_LITERAL(21, 256, 15), // "rcv_lower_slots"
QT_MOC_LITERAL(22, 272, 12), // "QDomElement*"
QT_MOC_LITERAL(23, 285, 4), // "elem"
QT_MOC_LITERAL(24, 290, 24), // "rcv_upper_register_slots"
QT_MOC_LITERAL(25, 315, 5), // "ss_id"
QT_MOC_LITERAL(26, 321, 4), // "port"
QT_MOC_LITERAL(27, 326, 26), // "rcv_upper_unregister_slots"
QT_MOC_LITERAL(28, 353, 8) // "uint16_t"

    },
    "session_layer\0snd_upper_signal\0\0"
    "ApplMessage*\0msg\0snd_upper_type_id_signal\0"
    "IntMap*\0type_ids\0snd_lower_signal\0"
    "QDomDocument*\0doc\0dst_ip\0dst_port\0"
    "snd_lower_register_signal\0sbs_ip\0"
    "sbs_port\0proto_type\0dev_port\0"
    "snd_lower_unregister_signal\0"
    "progress_log_signal\0log\0rcv_lower_slots\0"
    "QDomElement*\0elem\0rcv_upper_register_slots\0"
    "ss_id\0port\0rcv_upper_unregister_slots\0"
    "uint16_t"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_session_layer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       8,    3,   65,    2, 0x06 /* Public */,
      13,    4,   72,    2, 0x06 /* Public */,
      18,    2,   81,    2, 0x06 /* Public */,
      19,    1,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    1,   89,    2, 0x0a /* Public */,
      24,    5,   92,    2, 0x0a /* Public */,
      27,    3,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9, QMetaType::QString, QMetaType::UShort,   10,   11,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::QString,   20,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   25,   14,   26,   16,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, 0x80000000 | 28,   25,   14,   26,

       0        // eod
};

void session_layer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        session_layer *_t = static_cast<session_layer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->snd_upper_signal((*reinterpret_cast< ApplMessage*(*)>(_a[1]))); break;
        case 1: _t->snd_upper_type_id_signal((*reinterpret_cast< IntMap*(*)>(_a[1]))); break;
        case 2: _t->snd_lower_signal((*reinterpret_cast< QDomDocument*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 3: _t->snd_lower_register_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: _t->snd_lower_unregister_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->progress_log_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->rcv_lower_slots((*reinterpret_cast< QDomElement*(*)>(_a[1]))); break;
        case 7: _t->rcv_upper_register_slots((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 8: _t->rcv_upper_unregister_slots((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (session_layer::*_t)(ApplMessage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&session_layer::snd_upper_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (session_layer::*_t)(IntMap * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&session_layer::snd_upper_type_id_signal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (session_layer::*_t)(QDomDocument * , QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&session_layer::snd_lower_signal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (session_layer::*_t)(QString , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&session_layer::snd_lower_register_signal)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (session_layer::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&session_layer::snd_lower_unregister_signal)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (session_layer::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&session_layer::progress_log_signal)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject session_layer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_session_layer.data,
      qt_meta_data_session_layer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *session_layer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *session_layer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_session_layer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int session_layer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void session_layer::snd_upper_signal(ApplMessage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void session_layer::snd_upper_type_id_signal(IntMap * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void session_layer::snd_lower_signal(QDomDocument * _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void session_layer::snd_lower_register_signal(QString _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void session_layer::snd_lower_unregister_signal(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void session_layer::progress_log_signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
