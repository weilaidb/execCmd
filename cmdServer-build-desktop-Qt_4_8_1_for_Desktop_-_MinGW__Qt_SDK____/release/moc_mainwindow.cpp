/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue Jul 17 22:14:50 2018
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
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      88,   83,   11,   11, 0x08,
     125,  118,   11,   11, 0x08,
     145,   11,   11,   11, 0x08,
     167,  159,   11,   11, 0x08,
     198,  193,   11,   11, 0x08,
     228,   11,   11,   11, 0x08,
     260,   11,   11,   11, 0x08,
     295,  290,   11,   11, 0x08,
     333,   11,   11,   11, 0x08,
     343,   11,   11,   11, 0x08,
     375,  367,   11,   11, 0x08,
     397,  290,   11,   11, 0x08,
     429,   11,   11,   11, 0x08,
     471,  456,  451,   11, 0x08,
     496,   11,   11,   11, 0x08,
     523,   11,  515,   11, 0x08,
     535,   11,   11,   11, 0x08,
     554,   11,   11,   11, 0x08,
     574,   11,   11,   11, 0x08,
     601,  596,  515,   11, 0x08,
     628,   83,   11,   11, 0x08,
     648,   11,   11,   11, 0x08,
     675,  118,   11,   11, 0x08,
     701,   11,   11,   11, 0x08,
     736,  731,   11,   11, 0x08,
     777,  731,   11,   11, 0x08,
     814,  731,   11,   11, 0x08,
     864,   11,   11,   11, 0x08,
     888,   11,   11,   11, 0x08,
     921,   11,   11,   11, 0x08,
     945,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0sendstr\0on_connecting_sendstr(QString)\0"
    "on_pushButton_connect_clicked()\0text\0"
    "checkoneitem_execcmd(QString)\0ipaddr\0"
    "newConnect(QString)\0hellosocket()\0"
    "errmsg,\0procErrMsg(QString,void*)\0"
    "msg,\0readfromremote(QString,void*)\0"
    "on_pushButton_collect_clicked()\0"
    "on_pushButton_clear_clicked()\0item\0"
    "procDoubleClickItem(QListWidgetItem*)\0"
    "DelItem()\0updateListWidgetColor()\0"
    "findstr\0procFindList(QString)\0"
    "procEnterItem(QListWidgetItem*)\0"
    "procUseListTimerOut()\0bool\0curstr,setnull\0"
    "delKeyWord(QString,bool)\0procSaveTimerOut()\0"
    "QString\0getMapKey()\0procFindShortCut()\0"
    "procClearShortCut()\0procSendCmdShortCut()\0"
    "orgt\0filterInvalidText(QString)\0"
    "filterText(QString)\0procitemSelectionChanged()\0"
    "procComBoxIpList(QString)\0"
    "on_pushButton_paste_clicked()\0arg1\0"
    "on_comboBox_currentIndexChanged(QString)\0"
    "on_comboBox_editTextChanged(QString)\0"
    "on_comboBox_findlist_currentIndexChanged(QString)\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_push2box_clicked()\0"
    "CheckTodoListTimerOut()\0"
    "on_pushButton_delkey_clicked()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_connecting_sendstr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_connect_clicked(); break;
        case 2: _t->checkoneitem_execcmd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->newConnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->hellosocket(); break;
        case 5: _t->procErrMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 6: _t->readfromremote((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 7: _t->on_pushButton_collect_clicked(); break;
        case 8: _t->on_pushButton_clear_clicked(); break;
        case 9: _t->procDoubleClickItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->DelItem(); break;
        case 11: _t->updateListWidgetColor(); break;
        case 12: _t->procFindList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->procEnterItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 14: _t->procUseListTimerOut(); break;
        case 15: { bool _r = _t->delKeyWord((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: _t->procSaveTimerOut(); break;
        case 17: { QString _r = _t->getMapKey();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 18: _t->procFindShortCut(); break;
        case 19: _t->procClearShortCut(); break;
        case 20: _t->procSendCmdShortCut(); break;
        case 21: { QString _r = _t->filterInvalidText((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 22: _t->filterText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->procitemSelectionChanged(); break;
        case 24: _t->procComBoxIpList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->on_pushButton_paste_clicked(); break;
        case 26: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 27: _t->on_comboBox_editTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 28: _t->on_comboBox_findlist_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 29: _t->on_pushButton_clicked(); break;
        case 30: _t->on_pushButton_push2box_clicked(); break;
        case 31: _t->CheckTodoListTimerOut(); break;
        case 32: _t->on_pushButton_delkey_clicked(); break;
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
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
