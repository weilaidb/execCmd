/****************************************************************************
** Meta object code from reading C++ file 'sockthread.h'
**
** Created: Tue Jun 23 23:02:25 2020
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cmdServer/sockthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sockthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sockthread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,
      47,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   74,   11,   11, 0x0a,
     127,   11,   11,   11, 0x0a,
     160,  151,   11,   11, 0x0a,
     200,  194,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sockthread[] = {
    "sockthread\0\0,\0emitMsgDoneSignal(QString,void*)\0"
    "emitErrInfo(QString,void*)\0socketError\0"
    "displayErr(QAbstractSocket::SocketError)\0"
    "updateReadMsgProgress()\0numBytes\0"
    "updateWriteClientProgress(qint64)\0"
    "array\0ShowMsg(QByteArray)\0"
};

void sockthread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        sockthread *_t = static_cast<sockthread *>(_o);
        switch (_id) {
        case 0: _t->emitMsgDoneSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 1: _t->emitErrInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 2: _t->displayErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->updateReadMsgProgress(); break;
        case 4: _t->updateWriteClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 5: _t->ShowMsg((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sockthread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sockthread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_sockthread,
      qt_meta_data_sockthread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sockthread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sockthread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sockthread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sockthread))
        return static_cast<void*>(const_cast< sockthread*>(this));
    return QThread::qt_metacast(_clname);
}

int sockthread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void sockthread::emitMsgDoneSignal(QString _t1, void * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sockthread::emitErrInfo(QString _t1, void * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
