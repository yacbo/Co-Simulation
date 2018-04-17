/****************************************************************************
** Meta object code from reading C++ file 'server_proxy.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../server_proxy.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server_proxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_server_proxy_t {
    QByteArrayData data[13];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_server_proxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_server_proxy_t qt_meta_stringdata_server_proxy = {
    {
QT_MOC_LITERAL(0, 0, 12), // "server_proxy"
QT_MOC_LITERAL(1, 13, 16), // "snd_lower_signal"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 13), // "QDomDocument*"
QT_MOC_LITERAL(4, 45, 3), // "doc"
QT_MOC_LITERAL(5, 49, 6), // "dst_ip"
QT_MOC_LITERAL(6, 56, 4), // "port"
QT_MOC_LITERAL(7, 61, 16), // "ready_rcv_signal"
QT_MOC_LITERAL(8, 78, 12), // "ApplMessage*"
QT_MOC_LITERAL(9, 91, 3), // "msg"
QT_MOC_LITERAL(10, 95, 19), // "progress_log_signal"
QT_MOC_LITERAL(11, 115, 3), // "log"
QT_MOC_LITERAL(12, 119, 15) // "rcv_lower_slots"

    },
    "server_proxy\0snd_lower_signal\0\0"
    "QDomDocument*\0doc\0dst_ip\0port\0"
    "ready_rcv_signal\0ApplMessage*\0msg\0"
    "progress_log_signal\0log\0rcv_lower_slots"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_server_proxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       7,    1,   41,    2, 0x06 /* Public */,
      10,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::UShort,    4,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void server_proxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        server_proxy *_t = static_cast<server_proxy *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->snd_lower_signal((*reinterpret_cast< QDomDocument*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 1: _t->ready_rcv_signal((*reinterpret_cast< ApplMessage*(*)>(_a[1]))); break;
        case 2: _t->progress_log_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->rcv_lower_slots((*reinterpret_cast< ApplMessage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (server_proxy::*_t)(QDomDocument * , QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&server_proxy::snd_lower_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (server_proxy::*_t)(ApplMessage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&server_proxy::ready_rcv_signal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (server_proxy::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&server_proxy::progress_log_signal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject server_proxy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_server_proxy.data,
      qt_meta_data_server_proxy,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *server_proxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *server_proxy::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_server_proxy.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int server_proxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void server_proxy::snd_lower_signal(QDomDocument * _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void server_proxy::ready_rcv_signal(ApplMessage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void server_proxy::progress_log_signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
