/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Jun 18 01:02:12 2018
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cmdServer/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      90,   83,   11,   11, 0x08,
     110,   11,   11,   11, 0x08,
     132,  124,   11,   11, 0x08,
     163,  158,   11,   11, 0x08,
     193,   11,   11,   11, 0x08,
     225,   11,   11,   11, 0x08,
     260,  255,   11,   11, 0x08,
     298,   11,   11,   11, 0x08,
     308,   11,   11,   11, 0x08,
     340,  332,   11,   11, 0x08,
     362,  255,   11,   11, 0x08,
     394,   11,   11,   11, 0x08,
     416,   11,   11,   11, 0x08,
     443,   11,  435,   11, 0x08,
     455,   11,   11,   11, 0x08,
     474,   11,   11,   11, 0x08,
     494,   11,   11,   11, 0x08,
     521,  516,  435,   11, 0x08,
     548,   11,   11,   11, 0x08,
     561,   11,   11,   11, 0x08,
     588,   83,   11,   11, 0x08,
     614,   11,   11,   11, 0x08,
     649,  644,   11,   11, 0x08,
     690,  644,   11,   11, 0x08,
     727,  644,   11,   11, 0x08,
     777,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0sendstr\0on_connecting_sendstr(QString)\0"
    "on_pushButton_connect_clicked()\0ipaddr\0"
    "newConnect(QString)\0hellosocket()\0"
    "errmsg,\0procErrMsg(QString,void*)\0"
    "msg,\0readfromremote(QString,void*)\0"
    "on_pushButton_collect_clicked()\0"
    "on_pushButton_clear_clicked()\0item\0"
    "procDoubleClickItem(QListWidgetItem*)\0"
    "DelItem()\0updateListWidgetColor()\0"
    "findstr\0procFindList(QString)\0"
    "procEnterItem(QListWidgetItem*)\0"
    "procUseListTimerOut()\0procSaveTimerOut()\0"
    "QString\0getMapKey()\0procFindShortCut()\0"
    "procClearShortCut()\0procSendCmdShortCut()\0"
    "orgt\0filterInvalidText(QString)\0"
    "filterText()\0procitemSelectionChanged()\0"
    "procComBoxIpList(QString)\0"
    "on_pushButton_paste_clicked()\0arg1\0"
    "on_comboBox_currentIndexChanged(QString)\0"
    "on_comboBox_editTextChanged(QString)\0"
    "on_comboBox_findlist_currentIndexChanged(QString)\0"
    "on_pushButton_clicked()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_connecting_sendstr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_connect_clicked(); break;
        case 2: _t->newConnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->hellosocket(); break;
        case 4: _t->procErrMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 5: _t->readfromremote((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 6: _t->on_pushButton_collect_clicked(); break;
        case 7: _t->on_pushButton_clear_clicked(); break;
        case 8: _t->procDoubleClickItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->DelItem(); break;
        case 10: _t->updateListWidgetColor(); break;
        case 11: _t->procFindList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->procEnterItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 13: _t->procUseListTimerOut(); break;
        case 14: _t->procSaveTimerOut(); break;
        case 15: { QString _r = _t->getMapKey();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 16: _t->procFindShortCut(); break;
        case 17: _t->procClearShortCut(); break;
        case 18: _t->procSendCmdShortCut(); break;
        case 19: { QString _r = _t->filterInvalidText((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 20: _t->filterText(); break;
        case 21: _t->procitemSelectionChanged(); break;
        case 22: _t->procComBoxIpList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->on_pushButton_paste_clicked(); break;
        case 24: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: _t->on_comboBox_editTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->on_comboBox_findlist_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 27: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
