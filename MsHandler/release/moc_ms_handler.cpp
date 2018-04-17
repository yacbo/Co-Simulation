/****************************************************************************
** Meta object code from reading C++ file 'ms_handler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ms_handler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ms_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MsHandler_t {
    QByteArrayData data[13];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MsHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MsHandler_t qt_meta_stringdata_MsHandler = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MsHandler"
QT_MOC_LITERAL(1, 10, 25), // "ack_comm_cfg_param_signal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "PG_RTUI_Base*"
QT_MOC_LITERAL(4, 51, 4), // "data"
QT_MOC_LITERAL(5, 56, 12), // "login_signal"
QT_MOC_LITERAL(6, 69, 8), // "dev_name"
QT_MOC_LITERAL(7, 78, 6), // "dev_ip"
QT_MOC_LITERAL(8, 85, 8), // "uint16_t"
QT_MOC_LITERAL(9, 94, 8), // "dev_port"
QT_MOC_LITERAL(10, 103, 5), // "login"
QT_MOC_LITERAL(11, 109, 19), // "progress_log_signal"
QT_MOC_LITERAL(12, 129, 3) // "log"

    },
    "MsHandler\0ack_comm_cfg_param_signal\0"
    "\0PG_RTUI_Base*\0data\0login_signal\0"
    "dev_name\0dev_ip\0uint16_t\0dev_port\0"
    "login\0progress_log_signal\0log"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MsHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    4,   32,    2, 0x06 /* Public */,
      11,    1,   41,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 8, QMetaType::Bool,    6,    7,    9,   10,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void MsHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MsHandler *_t = static_cast<MsHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ack_comm_cfg_param_signal((*reinterpret_cast< PG_RTUI_Base*(*)>(_a[1]))); break;
        case 1: _t->login_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 2: _t->progress_log_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MsHandler::*_t)(PG_RTUI_Base * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MsHandler::ack_comm_cfg_param_signal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MsHandler::*_t)(QString , QString , uint16_t , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MsHandler::login_signal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MsHandler::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MsHandler::progress_log_signal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject MsHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MsHandler.data,
      qt_meta_data_MsHandler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MsHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MsHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MsHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MsHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MsHandler::ack_comm_cfg_param_signal(PG_RTUI_Base * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MsHandler::login_signal(QString _t1, QString _t2, uint16_t _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MsHandler::progress_log_signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
