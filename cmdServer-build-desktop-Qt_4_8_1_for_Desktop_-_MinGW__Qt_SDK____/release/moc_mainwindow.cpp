/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue Dec 4 23:09:08 2018
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
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      88,   83,   11,   11, 0x08,
     136,   83,   11,   11, 0x08,
     173,  166,   11,   11, 0x08,
     193,   11,   11,   11, 0x08,
     215,  207,   11,   11, 0x08,
     246,  241,   11,   11, 0x08,
     276,   11,   11,   11, 0x08,
     308,   11,   11,   11, 0x08,
     343,  338,   11,   11, 0x08,
     381,  338,   11,   11, 0x08,
     413,   11,   11,   11, 0x08,
     423,   11,   11,   11, 0x08,
     455,  447,   11,   11, 0x08,
     477,  447,   11,   11, 0x08,
     508,  338,   11,   11, 0x08,
     540,   11,   11,   11, 0x08,
     582,  567,  562,   11, 0x08,
     607,   11,   11,   11, 0x08,
     634,   11,  626,   11, 0x08,
     646,   11,   11,   11, 0x08,
     665,   11,   11,   11, 0x08,
     685,   11,   11,   11, 0x08,
     712,  707,  626,   11, 0x08,
     739,   83,   11,   11, 0x08,
     759,   11,   11,   11, 0x08,
     786,  166,   11,   11, 0x08,
     812,   11,   11,   11, 0x08,
     842,   11,  626,   11, 0x08,
     866,  861,   11,   11, 0x08,
     907,  861,   11,   11, 0x08,
     944,  861,   11,   11, 0x08,
     994,   11,   11,   11, 0x08,
    1018,   11,   11,   11, 0x08,
    1051,   11,   11,   11, 0x08,
    1075,   11,   11,   11, 0x08,
    1118,   11, 1106,   11, 0x08,
    1148, 1143,   11,   11, 0x08,
    1187,  447,   11,   11, 0x08,
    1227, 1222,   11,   11, 0x08,
    1267,   83,   11,   11, 0x08,
    1311,   11,   11,   11, 0x08,
    1341,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0sendstr\0on_connecting_sendstr(QString)\0"
    "on_pushButton_connect_clicked()\0text\0"
    "on_pushButton_connect_clicked_selftext(QString)\0"
    "checkoneitem_execcmd(QString)\0ipaddr\0"
    "newConnect(QString)\0hellosocket()\0"
    "errmsg,\0procErrMsg(QString,void*)\0"
    "msg,\0readfromremote(QString,void*)\0"
    "on_pushButton_collect_clicked()\0"
    "on_pushButton_clear_clicked()\0item\0"
    "procDoubleClickItem(QListWidgetItem*)\0"
    "procClickItem(QListWidgetItem*)\0"
    "DelItem()\0updateListWidgetColor()\0"
    "findstr\0procFindList(QString)\0"
    "procActivatedFindList(QString)\0"
    "procEnterItem(QListWidgetItem*)\0"
    "procUseListTimerOut()\0bool\0curstr,setnull\0"
    "delKeyWord(QString,bool)\0procSaveTimerOut()\0"
    "QString\0getMapKey()\0procFindShortCut()\0"
    "procClearShortCut()\0procSendCmdShortCut()\0"
    "orgt\0filterInvalidText(QString)\0"
    "filterText(QString)\0procitemSelectionChanged()\0"
    "procComBoxIpList(QString)\0"
    "on_pushButton_paste_clicked()\0"
    "getclipboardtext()\0arg1\0"
    "on_comboBox_currentIndexChanged(QString)\0"
    "on_comboBox_editTextChanged(QString)\0"
    "on_comboBox_findlist_currentIndexChanged(QString)\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_push2box_clicked()\0"
    "CheckTodoListTimerOut()\0"
    "on_pushButton_delkey_clicked()\0"
    "QStringList\0getcomuselistbycurkeys()\0"
    "list\0insertcomuselistbycurkeys(QStringList)\0"
    "insertfindkeys2comuselist(QString)\0"
    "maps\0printMapVar(QMap<QString,QStringList>&)\0"
    "on_pushButton_searchengine_clicked(QString)\0"
    "on_pushButton_baidu_clicked()\0"
    "on_pushButton_biying_clicked()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_connecting_sendstr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_connect_clicked(); break;
        case 2: _t->on_pushButton_connect_clicked_selftext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->checkoneitem_execcmd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->newConnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->hellosocket(); break;
        case 6: _t->procErrMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 7: _t->readfromremote((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 8: _t->on_pushButton_collect_clicked(); break;
        case 9: _t->on_pushButton_clear_clicked(); break;
        case 10: _t->procDoubleClickItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->procClickItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 12: _t->DelItem(); break;
        case 13: _t->updateListWidgetColor(); break;
        case 14: _t->procFindList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->procActivatedFindList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->procEnterItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 17: _t->procUseListTimerOut(); break;
        case 18: { bool _r = _t->delKeyWord((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: _t->procSaveTimerOut(); break;
        case 20: { QString _r = _t->getMapKey();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 21: _t->procFindShortCut(); break;
        case 22: _t->procClearShortCut(); break;
        case 23: _t->procSendCmdShortCut(); break;
        case 24: { QString _r = _t->filterInvalidText((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 25: _t->filterText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->procitemSelectionChanged(); break;
        case 27: _t->procComBoxIpList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->on_pushButton_paste_clicked(); break;
        case 29: { QString _r = _t->getclipboardtext();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 30: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 31: _t->on_comboBox_editTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->on_comboBox_findlist_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: _t->on_pushButton_clicked(); break;
        case 34: _t->on_pushButton_push2box_clicked(); break;
        case 35: _t->CheckTodoListTimerOut(); break;
        case 36: _t->on_pushButton_delkey_clicked(); break;
        case 37: { QStringList _r = _t->getcomuselistbycurkeys();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 38: _t->insertcomuselistbycurkeys((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 39: _t->insertfindkeys2comuselist((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 40: _t->printMapVar((*reinterpret_cast< QMap<QString,QStringList>(*)>(_a[1]))); break;
        case 41: _t->on_pushButton_searchengine_clicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 42: _t->on_pushButton_baidu_clicked(); break;
        case 43: _t->on_pushButton_biying_clicked(); break;
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
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
