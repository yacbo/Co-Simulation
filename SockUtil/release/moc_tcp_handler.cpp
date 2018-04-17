/****************************************************************************
** Meta object code from reading C++ file 'tcp_handler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tcp_handler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcp_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpNetHandler_t {
    QByteArrayData data[12];
    char stringdata0[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpNetHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpNetHandler_t qt_meta_stringdata_TcpNetHandler = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TcpNetHandler"
QT_MOC_LITERAL(1, 14, 15), // "new_net_handler"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 2), // "ip"
QT_MOC_LITERAL(4, 34, 4), // "port"
QT_MOC_LITERAL(5, 39, 13), // "EProtocolType"
QT_MOC_LITERAL(6, 53, 4), // "type"
QT_MOC_LITERAL(7, 58, 15), // "NetHandlerBase*"
QT_MOC_LITERAL(8, 74, 7), // "handler"
QT_MOC_LITERAL(9, 82, 12), // "receive_data"
QT_MOC_LITERAL(10, 95, 17), // "accept_connection"
QT_MOC_LITERAL(11, 113, 12) // "disconnected"

    },
    "TcpNetHandler\0new_net_handler\0\0ip\0"
    "port\0EProtocolType\0type\0NetHandlerBase*\0"
    "handler\0receive_data\0accept_connection\0"
    "disconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpNetHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   43,    2, 0x08 /* Private */,
      10,    0,   44,    2, 0x08 /* Private */,
      11,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::UShort, 0x80000000 | 5, 0x80000000 | 7,    3,    4,    6,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TcpNetHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TcpNetHandler *_t = static_cast<TcpNetHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->new_net_handler((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< EProtocolType(*)>(_a[3])),(*reinterpret_cast< NetHandlerBase*(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->receive_data(); break;
        case 2: _t->accept_connection(); break;
        case 3: _t->disconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< NetHandlerBase* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef bool (TcpNetHandler::*_t)(QString , quint16 , EProtocolType , NetHandlerBase * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpNetHandler::new_net_handler)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TcpNetHandler::staticMetaObject = {
    { &NetHandlerBase::staticMetaObject, qt_meta_stringdata_TcpNetHandler.data,
      qt_meta_data_TcpNetHandler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TcpNetHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpNetHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpNetHandler.stringdata0))
        return static_cast<void*>(this);
    return NetHandlerBase::qt_metacast(_clname);
}

int TcpNetHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NetHandlerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
bool TcpNetHandler::new_net_handler(QString _t1, quint16 _t2, EProtocolType _t3, NetHandlerBase * _t4)
{
    bool _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
