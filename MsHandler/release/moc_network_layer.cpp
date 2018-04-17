/****************************************************************************
** Meta object code from reading C++ file 'network_layer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../network_layer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'network_layer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_network_layer_t {
    QByteArrayData data[19];
    char stringdata0[217];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_network_layer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_network_layer_t qt_meta_stringdata_network_layer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "network_layer"
QT_MOC_LITERAL(1, 14, 16), // "snd_upper_signal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "QDomElement*"
QT_MOC_LITERAL(4, 45, 4), // "elem"
QT_MOC_LITERAL(5, 50, 19), // "progress_log_signal"
QT_MOC_LITERAL(6, 70, 3), // "log"
QT_MOC_LITERAL(7, 74, 15), // "rcv_upper_slots"
QT_MOC_LITERAL(8, 90, 13), // "QDomDocument*"
QT_MOC_LITERAL(9, 104, 3), // "doc"
QT_MOC_LITERAL(10, 108, 6), // "dst_ip"
QT_MOC_LITERAL(11, 115, 8), // "uint16_t"
QT_MOC_LITERAL(12, 124, 4), // "port"
QT_MOC_LITERAL(13, 129, 24), // "rcv_upper_register_slots"
QT_MOC_LITERAL(14, 154, 6), // "sbs_ip"
QT_MOC_LITERAL(15, 161, 8), // "sbs_port"
QT_MOC_LITERAL(16, 170, 10), // "proto_type"
QT_MOC_LITERAL(17, 181, 8), // "dev_port"
QT_MOC_LITERAL(18, 190, 26) // "rcv_upper_unregister_slots"

    },
    "network_layer\0snd_upper_signal\0\0"
    "QDomElement*\0elem\0progress_log_signal\0"
    "log\0rcv_upper_slots\0QDomDocument*\0doc\0"
    "dst_ip\0uint16_t\0port\0rcv_upper_register_slots\0"
    "sbs_ip\0sbs_port\0proto_type\0dev_port\0"
    "rcv_upper_unregister_slots"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_network_layer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    3,   45,    2, 0x0a /* Public */,
      13,    4,   52,    2, 0x0a /* Public */,
      18,    2,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString, 0x80000000 | 11,    9,   10,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   14,   12,

       0        // eod
};

void network_layer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        network_layer *_t = static_cast<network_layer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->snd_upper_signal((*reinterpret_cast< QDomElement*(*)>(_a[1]))); break;
        case 1: _t->progress_log_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->rcv_upper_slots((*reinterpret_cast< QDomDocument*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3]))); break;
        case 3: _t->rcv_upper_register_slots((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: _t->rcv_upper_unregister_slots((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (network_layer::*_t)(QDomElement * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&network_layer::snd_upper_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (network_layer::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&network_layer::progress_log_signal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject network_layer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_network_layer.data,
      qt_meta_data_network_layer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *network_layer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *network_layer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_network_layer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int network_layer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void network_layer::snd_upper_signal(QDomElement * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void network_layer::progress_log_signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
