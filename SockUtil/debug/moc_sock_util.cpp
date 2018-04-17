/****************************************************************************
** Meta object code from reading C++ file 'sock_util.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sock_util.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sock_util.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SockUtil_t {
    QByteArrayData data[9];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SockUtil_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SockUtil_t qt_meta_stringdata_SockUtil = {
    {
QT_MOC_LITERAL(0, 0, 8), // "SockUtil"
QT_MOC_LITERAL(1, 9, 19), // "add_new_net_handler"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 2), // "ip"
QT_MOC_LITERAL(4, 33, 4), // "port"
QT_MOC_LITERAL(5, 38, 13), // "EProtocolType"
QT_MOC_LITERAL(6, 52, 4), // "type"
QT_MOC_LITERAL(7, 57, 15), // "NetHandlerBase*"
QT_MOC_LITERAL(8, 73, 7) // "handler"

    },
    "SockUtil\0add_new_net_handler\0\0ip\0port\0"
    "EProtocolType\0type\0NetHandlerBase*\0"
    "handler"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SockUtil[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, 0x80000000 | 5, 0x80000000 | 7,    3,    4,    6,    8,

       0        // eod
};

void SockUtil::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SockUtil *_t = static_cast<SockUtil *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->add_new_net_handler((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< EProtocolType(*)>(_a[3])),(*reinterpret_cast< NetHandlerBase*(*)>(_a[4]))); break;
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
    }
}

const QMetaObject SockUtil::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SockUtil.data,
      qt_meta_data_SockUtil,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SockUtil::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SockUtil::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SockUtil.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SockUtil::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
