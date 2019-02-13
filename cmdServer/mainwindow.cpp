#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QCloseEvent>
#include <QRegExp>
#include "msgtips.h"
#include "sockthread.h"
#include <QMenu>
#include <QShortcut>
#include "version.h"
#include <QClipboard>
#include <windows.h>
#include <QTextCodec>
#include <QFileDialog>
#include <QFile>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QMessageBox>

#define BINDPORT (9999)
#define CMD_HELLO "hello word"
#define KEY_DELETEWORD "delete it"
#define LOCALHOSTIPADDR ("127.0.0.1")
#define DEFAULT_LOADTREE_DIR "c:\\"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pthreadsock(NULL),
    uselistTimer(NULL),
    savetimer(NULL),
    bCtrlKeyPressed(FALSE),
    model(NULL),
    curmodeldir(""),
    orglabeltext(""),
    cursavefilepath(""),
    cliptexttimer(NULL)
{
    ui->setupUi(this);

    //splitte�ָ����
    ui->splitter->setStretchFactor(0,2);
    ui->splitter->setStretchFactor(1,8);

    //splitte�ָ����
    ui->splitter_2->setStretchFactor(0,5);
    ui->splitter_2->setStretchFactor(1,5);


    mapIpAndContent.clear();
    map_showcmd.clear();

    ReadHistorySettings();
    UpdateShowCmdListWidgetByMap();
    initDirTree(curmodeldir);
    orglabeltext = ui->label->text();

    on_checkBox_tree_toggled(ui->checkBox_tree->isChecked());

    ui->comboBox_findlist->addItems(getcomuselistbycurkeys());
    publicSets();
    PopMenu();
    autosendstr.clear();
    ui->lineEdit_sendnum->setValidator(new QIntValidator(0, 1000, this)); ;
    ui->lineEdit_sendnum->setPlaceholderText(QString::fromLocal8Bit("����"));
    ui->lineEdit_sendnum->hide();
    ui->statusBar->showMessage(QString::fromLocal8Bit(version));

    uselistTimer = new QTimer();
    uselistTimer->start(10000);
    connect(uselistTimer, SIGNAL(timeout()), this, SLOT(procUseListTimerOut()));


    savetimer = new QTimer();
    savetimer->start(10000);
    connect(savetimer, SIGNAL(timeout()), this, SLOT(procSaveTimerOut()));


    //    ui->comboBox_findlist->setShortcutEnabled();
    //    ui->comboBox_findlist->setShortcutAutoRepeat();
    //     ui->udpSendButton->setShortcut(tr("Alt+F"));

    QShortcut *findShortCut = new QShortcut(this);
    findShortCut->setKey(tr("Ctrl+F"));
    connect(findShortCut, SIGNAL(activated()),this,SLOT(procFindShortCut()));

    //     QShortcut *findClearShortCut = new QShortcut(this);
    //     findClearShortCut->setKey(tr("Ctrl+D"));
    //     connect(findClearShortCut, SIGNAL(activated()),this,SLOT(procClearShortCut()));

    QShortcut *SendCmdShortCut = new QShortcut(this);
    SendCmdShortCut->setKey(tr("Ctrl+S"));
    connect(SendCmdShortCut, SIGNAL(activated()),this,SLOT(procSendCmdShortCut()));


    cliptexttimer = new QTimer();
    cliptexttimer->start(1000);
    connect(cliptexttimer, SIGNAL(timeout()), this, SLOT(CheckDictTimerOut()));



    TodoCmdExecList.clear();
    exectimer = new QTimer();
    exectimer->start(2000);
    connect(exectimer, SIGNAL(timeout()), this, SLOT(CheckTodoListTimerOut()));


    //     setPlaceholderText()������ʾ����
    //     QLineEdit *edit;
    //     edit->setPlaceholderText("niahoma");

    //ǿ��IP��־
    forceipaddrflag = false;


    // �����������źź��Զ���Ĳ۽��й���
    //     connect(&myWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(procDirChanged(QString)));
    connect(&myWatcher, SIGNAL(fileChanged(QString)), this, SLOT(procDirChanged(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::publicSets()
{

    ui->listWidget_cmdlist->setMouseTracking(true);
    connect(ui->listWidget_cmdlist, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(procDoubleClickItem(QListWidgetItem *)));
    connect(ui->listWidget_cmdlist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(procClickItem(QListWidgetItem *)));
    connect(ui->listWidget_cmdlist, SIGNAL(itemEntered(QListWidgetItem*)),this, SLOT(procEnterItem(QListWidgetItem*)));
    connect(ui->listWidget_cmdlist, SIGNAL(itemSelectionChanged()), this, SLOT(procitemSelectionChanged()));
    connect(ui->comboBox_findlist, SIGNAL(editTextChanged(QString)), this, SLOT(procFindList(QString)));
    connect(ui->comboBox_findlist, SIGNAL(activated(QString)), this, SLOT(procActivatedFindList(QString)));


}

void MainWindow::Var2Map(QSettings &sets, QString envkey, QMap<QString, QStringList> &outmap)
{
    QMap<QString, QVariant> maptmp = sets.value(envkey).toMap();

    //map_showcmd
    QMapIterator<QString, QVariant> i(maptmp);
    while (i.hasNext()) {
        i.next();
        QString key = i.key();
        QVariant val = i.value();
        outmap.insert(key, val.toStringList());
    }
}

QMap<QString, QVariant> MainWindow::Map2Var(QMap<QString, QStringList> &inmap)
{
    QMap<QString, QVariant> maptmp;
    maptmp.clear();
    QMapIterator<QString, QStringList> i(inmap);
    while (i.hasNext()) {
        i.next();
        QString key = i.key();
        QVariant val = i.value();
        maptmp.insert(key, val);
    }
    return maptmp;
}





/*============================================
* FuncName    : autoCCode::ReadHistorySettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::ReadHistorySettings()
{
    QSettings m_settings("weilaidb.com.cn", "cmdserver");
    ui->comboBox->addItems(m_settings.value("ComBoxIPList").toStringList());
    ComBoxIPList = m_settings.value("ComBoxIPList").toStringList();
    ui->comboBox->setEditText(m_settings.value("curkey").toString());
    curmodeldir = m_settings.value("curmodeldir").toString();
    //    show_cmdlist.clear();
    //    show_cmdlist = m_settings.value("listWidget_cmdlist").toStringList();

    Var2Map(m_settings, "map_showcmd", map_showcmd);
    Var2Map(m_settings, "map_commonuselist", map_commonuselist);

    //    mapIpAndContent
    //    ui->lineEdit->setText(m_settings.value("LineEditIP").toString());
    ui->checkBox_autosend->setChecked(m_settings.value("checkBox_autosend").toBool());
    ui->checkBox_fileautoload->setChecked(m_settings.value("checkBox_fileautoload").toBool());
    ui->checkBox_tree->setChecked(m_settings.value("checkBox_tree").toBool());
    ui->checkBox_dict_realtime->setChecked(m_settings.value("checkBox_dict_realtime").toBool());
    this->restoreGeometry(m_settings.value("Cmdserver").toByteArray());

}

/*============================================
* FuncName    : autoCCode::WriteCurrentSettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::WriteCurrentSettings()
{
    QSettings m_settings("weilaidb.com.cn", "cmdserver");
    m_settings.setValue("ComBoxIPList",ComBoxIPList);
    m_settings.setValue("curkey", ui->comboBox->currentText());
    m_settings.setValue("curmodeldir", curmodeldir);
    //    show_cmdlist.clear();
    //    int count = ui->listWidget_cmdlist->count();
    //    int loop = 0;
    //    for(loop = 0; loop < count;loop++)
    //    {
    //        cmdlist << ui->listWidget_cmdlist->item(loop)->text();
    //    }
    m_settings.setValue("listWidget_cmdlist",show_cmdlist);

    m_settings.setValue("map_showcmd", Map2Var(map_showcmd));
    m_settings.setValue("map_commonuselist", Map2Var(map_commonuselist));


    m_settings.setValue("checkBox_autosend",ui->checkBox_autosend->isChecked());
    m_settings.setValue("checkBox_fileautoload",ui->checkBox_fileautoload->isChecked());
    m_settings.setValue("checkBox_tree",ui->checkBox_tree->isChecked());
    m_settings.setValue("checkBox_dict_realtime",ui->checkBox_dict_realtime->isChecked());


    m_settings.setValue("Cmdserver", this->saveGeometry());

    //    QSettings::SettingsMap
    //    QSettings::SettingsMap
    //    m_settings.setValue("mapIpAndContent",mapIpAndContent);


}


/*============================================
* FuncName    : MainWindow::closeEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent";
    WriteCurrentSettings();
    printMapVar(map_commonuselist);
    event->accept();
}


int MainWindow::CheckIPAddr(QString ipaddr)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!regExp.exactMatch(ipaddr))
    {
        ShowTipsInfo(str_china("��ʾ, ip��ַ����"));
        //        logsappendShow(QString("ip %1  invalid!!!").arg(ipaddr));
        return -1;
    }

    return 0;
}

void MainWindow::on_connecting_sendstr(QString sendstr)
{
    on_pushButton_connect_clicked();
    return;

    int times = ui->lineEdit_sendnum->text().toInt();
    int loop = 0;
    if(ui->lineEdit_sendnum->text().toInt() == 0 )
    {
        on_pushButton_connect_clicked();
    }
    else
    {
        //        for(loop = 0; loop < times; loop++)
        //        {
        //           on_pushButton_connect_clicked();
        //        }
    }
}

void MainWindow::procComBoxIpList(QString ipaddr)
{
    foreach (QString inner, ComBoxIPList) {
        if(inner == ipaddr)
        {
            return;
        }
    }
    ComBoxIPList.append(ipaddr);
    ComBoxIPList.sort();
    ComBoxIPList.removeDuplicates();

    ui->comboBox->clear();
    ui->comboBox->addItems(ComBoxIPList);
    ui->comboBox->setEditText(ipaddr);
}

void MainWindow::checkoneitem_execcmd(QString text)
{
    filterText(text);
    QString ipaddr = ui->comboBox->currentText();
    if(0 != CheckIPAddr(ipaddr))
    {
        return;
    }

    if(forceipaddrflag || (ui->checkBox_forcelocalhost->isChecked()))
    {
        ipaddr = LOCALHOSTIPADDR;
    }

    procComBoxIpList(ipaddr);

    ui->pushButton_connect->setEnabled(false);
    ui->textEdit_cmdresult->clear();

    socket = new QTcpSocket();

    newConnect(ipaddr);


    qDebug() << "on connecting";
}

void MainWindow::on_pushButton_connect_clicked()
{
    checkoneitem_execcmd(ui->textEdit->toPlainText());
}

void MainWindow::on_pushButton_connect_clicked_selftext(QString text)
{
    checkoneitem_execcmd(text);
}

void MainWindow::newConnect(QString ipaddr)
{
    qDebug() << "-->>client socket:" << socket;
    if(socket)
    {
        socket->disconnectFromHost();
        socket->abort();
        socket->close();
        socket = NULL;
        if(pthreadsock)
        {
            pthreadsock->deleteLater();
            pthreadsock = NULL;
        }
    }
    ShowTipsInfoWithShowTime(str_china("����������..."), 2000);
    qApp->processEvents();

    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);//�Ż�Ϊ����ӳ٣������1�������ø��Ż���
    socket->connectToHost(ipaddr, BINDPORT);
    connect(socket, SIGNAL(connected()), this, SLOT(hellosocket()));




    qDebug() <<"socket err:" << socket->errorString();
    qDebug() <<"remote ip addr:" << ipaddr;


    //������falseʱ���Ե���error��ȷ���޷����ӵ�ԭ��
    if(!socket->waitForConnected(3000))
    {
        ShowTipsInfoWithShowTime(str_china("���ӳ�ʱ�������������ַ�Ƿ���ȷ"), 2000);
        ui->pushButton_connect->setEnabled(true);
        //        ����󣬹㲥����sever
        //        BroadcastGetIpCommand();
        return;
    }



}

/*============================================
* FuncName    : autoCCode::hellosocket
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::hellosocket()
{
    ShowTipsInfo(QString::fromLocal8Bit("���ӷ������ɹ�����"));

    if(pthreadsock == NULL)
    {
        pthreadsock = new sockthread(this);
        pthreadsock->setSocketConnect(socket);
        QObject::connect(pthreadsock,SIGNAL(emitMsgDoneSignal(QString,void*)),
                         this,SLOT(readfromremote(QString,void*)));
        QObject::connect(pthreadsock,SIGNAL(emitErrInfo(QString,void*)),
                         this,SLOT(procErrMsg(QString,void*)));
        pthreadsock->start();
        if(autosendstr.length())
        {
            pthreadsock->sendmsg(autosendstr);
            autosendstr.clear();
        }
        else if(ui->textEdit->toPlainText().length() == 0)
            pthreadsock->sendmsg(CMD_HELLO);
        else
            pthreadsock->sendmsg(ui->textEdit->toPlainText());
    }



}

void MainWindow::procErrMsg(QString errmsg,void*)
{
    //    ShowTipsInfo(errmsg);
    qDebug()<< errmsg;
    ui->pushButton_connect->setEnabled(true);
    if(socket)
    {
        socket->disconnectFromHost();
        socket->abort();
        socket->close();
        socket = NULL;
    }
}

void MainWindow::readfromremote(QString msg,void*)
{
    qDebug() << "read from remote";
    qDebug() << msg;
    //    ui->textEdit_cmdresult->setText(msg);
    ui->textEdit_cmdresult->append(QString::fromUtf8(msg.toAscii().data()));
    //    ui->textEdit_cmdresult->append(QString::fromLocal8Bit(msg.toAscii().data()));
}


void MainWindow::updateListWidgetColor()
{
    int count = ui->listWidget_cmdlist->count();
    int loop = 0;
    for(loop = 0; loop < count;loop++)
    {
        if(loop % 2 )
            ui->listWidget_cmdlist->item(loop)->setForeground(QColor(Qt::black));
        else
            ui->listWidget_cmdlist->item(loop)->setForeground(QColor(Qt::blue));
    }
}

void MainWindow::on_pushButton_collect_clicked()
{
    qDebug() << "on_pushButton_collect_clicked";
    if(ui->textEdit->toPlainText().length() == 0)
    {
        return;
    }
    if(getMapKey().isEmpty())
    {
        qDebug() << "get map key empty!!";
        return;
    }

    QString oldstring = ui->textEdit->toPlainText();

    QMapIterator<QString, QStringList> i(map_showcmd);

    quint8 found = FALSE;
    QStringList findlist;
    findlist.clear();
    while (i.hasNext()) {
        i.next();
        if(i.key() == getMapKey())
        {
            //            qDebug() << i.value();
            found = TRUE;
            findlist = i.value();
            break;
        }
    }
    if(found == FALSE)
    {
        QStringList nulllist;
        nulllist.clear();
        map_showcmd.insert(getMapKey(), nulllist);
        findlist = nulllist;
    }

    QString currenttext = ui->textEdit->toPlainText();

    foreach (QString str, findlist) {
        if(str == currenttext)
            return;
    }

    findlist << currenttext;
    map_showcmd.insert(getMapKey(), findlist);

    UpdateShowCmdListWidgetByMap();

    procFindList(ui->comboBox_findlist->currentText());

    ui->textEdit->setText(oldstring); //�ָ�ԭ���ı�����ʾ

}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    ui->textEdit_cmdresult->clear();

    TodoCmdExecList.clear();
    searchlist.clear();
    oneshowres.clear();
    ui->label->setText(orglabeltext);
}

void MainWindow::procDoubleClickItem(QListWidgetItem * item)
{
    //    if(ui->checkBox_autosend->isChecked())
    {
        autosendstr = item->text();
        ui->textEdit->setText(item->text());
        on_connecting_sendstr("");

        return;
    }
    //    qDebug() << item->text();
    ui->textEdit->setText(item->text());
}

void MainWindow::procClickItem(QListWidgetItem * item)
{
    if(ui->checkBox_autosend->isChecked())
    {
        autosendstr = item->text();
        ui->textEdit->setText(item->text());
        on_connecting_sendstr("");

        return;
    }
    //    qDebug() << item->text();
    ui->textEdit->setText(item->text());
}


void MainWindow::procEnterItem(QListWidgetItem* item)
{
    QString itemtext = item->text();
    //    qDebug() << "enter " << itemtext;
    setToolTip(itemtext);
    //    ui->textEdit->setText(itemtext);
}

void MainWindow::procitemSelectionChanged()
{
    QString curtext = ui->listWidget_cmdlist->currentItem()->text();
    setToolTip(curtext);
    ui->textEdit->setText(curtext);
}

void MainWindow::procFindList(QString findstr)
{
    oneshowres.clear();
    mutex_search.lock();
    if(ui->comboBox_findlist->currentText().simplified().length() == 0)
    {
        UpdateShowCmdListWidgetByMap();
        mutex_search.unlock();
        return;
    }
    searchlist.clear();
    QStringList splitlist = findstr.split(QRegExp("\\s"));
    int size = splitlist.size();
    int count = 0;
    QStringList curcmdlist = GetCurrentMapValue();
    foreach (QString str, curcmdlist) {
        count = 0;
        foreach (QString splitstr, splitlist) {
            if(!str.contains(splitstr, Qt::CaseInsensitive))
                break;
            count++;
        }
        if(size == count)
        {
            searchlist << str;
        }
    }

    //    searchlist.sort();
    UpdateShowCmdListWidget(findstr, searchlist);
    mutex_search.unlock();

}

void MainWindow::procActivatedFindList(QString findstr)
{
    qDebug() << "procActivatedFindList, findstr:" << findstr;
    findstr = findstr;

    if(searchlist.size() > 0)
    {
        //        ui->textEdit->setText(list.at(0));
        checkoneitem_execcmd(oneshowres);
        insertfindkeys2comuselist(findstr);
    }
}


/* �Ҽ��˵� */
/*============================================
* FuncName    : autoCCode::contextMenuEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QCursor cur=this->cursor();
    QMenu *menu=new QMenu(this);
    menu->addAction(Act_DelItem); //��Ӳ˵���1
    menu->addAction(Act_LoadDirTree); //��Ӳ˵���1
    menu->addAction(Act_Mkdir); //��Ӳ˵���1
    menu->addAction(Act_Rmdir); //��Ӳ˵���1
    //    menu->addAction(Act_Normal); //��Ӳ˵���2
    menu->exec(cur.pos()); //���������



}

void MainWindow::PopMenu()
{
    /* �Ҽ��˵� */
    Act_DelItem = new QAction(tr("DelItem"), this);
    connect(Act_DelItem, SIGNAL(triggered()), this, SLOT(DelItem()));
    /* ������ʾ���ε�·�� */
    Act_LoadDirTree = new QAction(tr("LoadTree"), this);
    connect(Act_LoadDirTree, SIGNAL(triggered()), this, SLOT(LoadDirTree()));
    /* ����Ŀ¼ */
    Act_Mkdir = new QAction(tr("Make Directory..."), this);
    connect(Act_Mkdir, SIGNAL(triggered()), this, SLOT(TreeMkdir()));

    /* ɾ��Ŀ¼ */
    Act_Rmdir = new QAction(tr("Remove"), this);
    connect(Act_Rmdir, SIGNAL(triggered()), this, SLOT(TreeRm()));



    SearchMenu();
}

void MainWindow::SearchMenu()
{
#define GBKSELF(A) QString::fromLocal8Bit(#A)


    /* �����˵� */
    Act_searchbaidu = new QAction(GBKSELF(�ٶ�), this);
    connect(Act_searchbaidu, SIGNAL(triggered()), this, SLOT(on_pushButton_baidu_clicked()));
    Act_searchbiying = new QAction(GBKSELF(��Ӧ), this);
    connect(Act_searchbiying, SIGNAL(triggered()), this, SLOT(on_pushButton_biying_clicked()));
    Act_search360so = new QAction(GBKSELF(360��), this);
    connect(Act_search360so, SIGNAL(triggered()), this, SLOT(on_pushButton_360so_clicked()));
    Act_notepadpp = new QAction(GBKSELF(notepad++), this);
    connect(Act_notepadpp, SIGNAL(triggered()), this, SLOT(on_pushButton_notepadpp_clicked()));

    Act_saveresult = new QAction(GBKSELF(����), this);
    connect(Act_saveresult, SIGNAL(triggered()), this, SLOT(on_pushButton_saveresult_clicked()));

    Act_saveresult_2another = new QAction(GBKSELF(���Ϊ), this);
    connect(Act_saveresult_2another, SIGNAL(triggered()), this, SLOT(on_pushButton_saveresult2_another_clicked()));


//    Act_openfile2result = new QAction(GBKSELF(��), this);
//    connect(Act_openfile2result, SIGNAL(triggered()), this, SLOT(on_pushButton_openfile2result_clicked()));




    /* �����˵� **/
    QMenu *menu=new QMenu(this);
    menu->addAction(Act_notepadpp); //��Ӳ˵���1
    menu->addAction(Act_searchbaidu); //��Ӳ˵���1
    menu->addAction(Act_searchbiying); //��Ӳ˵���1
    menu->addAction(Act_search360so); //��Ӳ˵���1
//    menu->addAction(Act_openfile2result); //��Ӳ˵���1
    menu->addAction(Act_saveresult); //��Ӳ˵���1
    menu->addAction(Act_saveresult_2another); //��Ӳ˵���1
    ui->pushButton_baidu->setMenu(menu);

}


void MainWindow::DelItem()
{
    if(ui->listWidget_cmdlist->currentRow() < 0)
    {
        return;
    }
    QStringList cmdlist = GetCurrentMapValue();
    cmdlist.removeOne(ui->listWidget_cmdlist->currentItem()->text());
    map_showcmd.insert(getMapKey(), cmdlist);
    if(ui->listWidget_cmdlist->count() == 1)
    {
        ui->listWidget_cmdlist->clear();
    }
    else
    {
        ui->listWidget_cmdlist->takeItem(ui->listWidget_cmdlist->currentRow());
        ui->listWidget_cmdlist->sortItems();
    }
}

bool MainWindow::delKeyWord(QString curstr, bool setnull)
{
    QString delstr = KEY_DELETEWORD;
    if(curstr.contains(delstr))
    {
        QString todel = curstr;
        todel = todel.replace(delstr, "").simplified();
        //        qDebug() << "!!!procUseListTimerOut del uselist:" << todel;
        comuseitemlist = getcomuselistbycurkeys();
        comuseitemlist.removeOne(todel);
        comuseitemlist.sort();
        comuseitemlist.removeDuplicates();
        insertcomuselistbycurkeys(comuseitemlist);
        ui->comboBox_findlist->clear();
        ui->comboBox_findlist->addItems(getcomuselistbycurkeys());
        if(setnull)
        {
            ui->comboBox_findlist->setEditText("");
        }
        else
        {
            ui->comboBox_findlist->setEditText(curstr);
        }
        ui->comboBox_findlist->update();
        return TRUE;
    }
    return FALSE;
}

void MainWindow::procUseListTimerOut()
{
    return;
    static QString oldstr = ui->comboBox_findlist->currentText().simplified();
    QString curstr = ui->comboBox_findlist->currentText().simplified();
    //    qDebug() << "oldstr:" << oldstr;
    //    qDebug() << "curstr:" << curstr;
    if(oldstr != curstr)
    {
        oldstr = curstr;
        return;
    }
    if(delKeyWord(curstr,FALSE))
    {
        return;
    }
    if(ui->listWidget_cmdlist->count() == 0 )
    {
        return;
    }
    qDebug() << "!!!procUseListTimerOut add uselist:" << curstr;
    comuseitemlist = getcomuselistbycurkeys();
    comuseitemlist.append(curstr);
    comuseitemlist.sort();
    comuseitemlist.removeDuplicates();
    insertcomuselistbycurkeys(comuseitemlist);
    ui->comboBox_findlist->clear();
    ui->comboBox_findlist->addItems(getcomuselistbycurkeys());
    ui->comboBox_findlist->setEditText(curstr);
    ui->comboBox_findlist->update();
}

void MainWindow::procFindShortCut()
{
    qDebug() << "procFindShortCut";
    ui->comboBox_findlist->setEditText("");
    ui->comboBox_findlist->setFocus();
    ui->label->setText(orglabeltext);
}
void MainWindow::procClearShortCut()
{
    qDebug() << "procClearShortCut";
    ui->comboBox_findlist->setEditText("");
}
void MainWindow::procSendCmdShortCut()
{
    qDebug() << "procSendCmdShortCut";
    if(ui->pushButton_connect->isEnabled())
    {
        on_pushButton_connect_clicked();
    }
}

void MainWindow::filterText(QString text)
{
    autosendstr.clear();
    autosendstr = filterInvalidText(text);
    qDebug() << "autosendstr:" << autosendstr;
}

QString MainWindow::filterInvalidText(QString orgt)
{
    //�����硾,[,# ��ʼ������
    QStringList list = orgt.split("\n");
    QString result = "";
    int lastline = 0;
    foreach (QString line, list) {
        QString dealline = line.simplified();
        if(dealline.contains(QRegExp("^\\#"))
                ||dealline.contains(QRegExp("^\\["))
                )
        {
            continue;
        }


#define BREAKSIGN  0x3010  // �����˷��� ��  unicode����
        ushort uni = 0;
        int nCount = dealline.count();
        for(int i = 0 ; i < nCount ; i++)
        {
            QChar cha = dealline.at(i);
            uni = cha.unicode();
            //            qDebug() << "uni:" << uni;
            //�������ַ http://www.qqxiuzi.cn/bianma/zifuji.php
            if(uni == BREAKSIGN) // unicode����
            {
                qDebug() << "break uni:" << uni;
                break;
            }
            if(uni >= 0x4E00 && uni <= 0x9FA5)
            {
                //����ַ�������
            }
        }
        //        qDebug() << "uni last:" << uni;
        if(uni == BREAKSIGN)// unicode����
        {
            continue;
        }

        if(++lastline == list.size())
        {
            result += line;
        }
        else
        {
            result += line + "\n";
        }
    }
    return result;
}




void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(!bCtrlKeyPressed)
        return;
    int numberDegrees = event->delta() / 8;
    int numberSteps = numberDegrees / 15;
    if(event->orientation() == Qt::Vertical)
    {   //ʵ�ֵ��Ǻ����ƶ�����������ѹ��ֵ������ƶ�ʵ��Ϊ
        //        ui->listWidge `Q1SW23t->horizontalScrollBar()->setValue(ui->listWidget->horizontalScrollBar()->value() + numberSteps);
    }
    qDebug() << "numberSteps :"<<numberSteps;
    if(numberSteps > 0)
    {
        QFont font = ui->textEdit->font();
        font.setPointSize(font.pointSize() + 1);

        ui->textEdit->setFont(font);
    }
    else
    {
        QFont font = ui->textEdit->font();
        font.setPointSize(font.pointSize() - 1);
        ui->textEdit->setFont(font);
    }
    event->accept();

}

void MainWindow::keyPressEvent(QKeyEvent *keyevt)
{
    switch(keyevt->key())
    {
    case Qt::Key_Control:
        bCtrlKeyPressed = TRUE;
        qDebug() << "bCtrlKeyPressed TRUE";
        break;
    default:
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *keyevt)
{
    switch(keyevt->key())
    {
    case Qt::Key_Control:
        bCtrlKeyPressed = FALSE;
        qDebug() << "bCtrlKeyPressed FALSE";
        break;
    default:
        break;
    }
}


void MainWindow::on_pushButton_paste_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui->textEdit->setText(clipboard->text());
}

QString MainWindow::getclipboardtext()
{
    QClipboard *clipboard = QApplication::clipboard();
    return clipboard->text();
}


void MainWindow::UpdateShowCmdListWidget(QString findstr, QStringList list)
{
    oneshowres.clear();
    ui->listWidget_cmdlist->clear();
    foreach (QString item, list) {
        QListWidgetItem *lst = new QListWidgetItem(QIcon("images/floder.png"),
                                                   item,
                                                   ui->listWidget_cmdlist);
        QStringList countstr = item.split("\n");
        int showheight = (countstr.size() >= 4) ? 4 : countstr.size();
        if(showheight == 0)
            showheight = 1;
        lst->setSizeHint(QSize(200,showheight * 20));

    }
    ui->listWidget_cmdlist->sortItems();
    //show first item if find result
    qDebug() << "find list result size:" << list.size();
    if(list.size() > 0)
    {
        oneshowres = ui->listWidget_cmdlist->item(0)->text();
        qDebug() << "list.at(0):" << list.at(0);
        ui->textEdit->setText(oneshowres);
    }
    else
    {
        ui->textEdit->setText("");
    }
    //    updateListWidgetColor();
}

void MainWindow::UpdateShowCmdListWidgetByMap()
{
    ui->listWidget_cmdlist->clear();
    QMapIterator<QString, QStringList> i(map_showcmd);

    quint8 found = FALSE;
    QStringList findlist;
    findlist.clear();
    while (i.hasNext()) {
        i.next();
        if(i.key() == getMapKey())
        {
            //            qDebug() << i.value();
            found = TRUE;
            findlist = i.value();
            break;
        }
    }
    if(found == FALSE)
    {
        return;
    }

    foreach (QString item, findlist) {
        QListWidgetItem *lst = new QListWidgetItem(QIcon("images/floder.png"),
                                                   item,
                                                   ui->listWidget_cmdlist);
        QStringList countstr = item.split("\n");
        int showheight = (countstr.size() >= 4) ? 4 : countstr.size();
        if(showheight == 0)
            showheight = 1;
        lst->setSizeHint(QSize(200,showheight * 20));

    }
    ui->listWidget_cmdlist->sortItems();

    //    qDebug() << "ui->listWidget_cmdlist count:" << ui->listWidget_cmdlist->count();

    //    if(findlist.size() == 1)
    //    {
    //        ui->textEdit_cmdresult->setText(findlist.at(0));
    //    }


}


QStringList MainWindow::GetCurrentMapValue()
{
    QMapIterator<QString, QStringList> i(map_showcmd);

    QStringList findlist;
    findlist.clear();
    while (i.hasNext()) {
        i.next();
        if(i.key() == getMapKey())
        {
            //            qDebug() << i.value();
            findlist = i.value();
            break;
        }
    }
    return findlist;
}

void MainWindow::procSaveTimerOut()
{
    //    qDebug() << "procSaveTimerOut...";
    //    static int orgcount = show_cmdlist.size();
    //    if(show_cmdlist.size() == orgcount)
    //    {
    //        return;
    //    }
    //    orgcount = show_cmdlist.size();
    WriteCurrentSettings();
}


QString MainWindow::getMapKey()
{
    return ui->comboBox->currentText().simplified();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->listWidget_cmdlist->clear();
        return;
    }
    UpdateShowCmdListWidgetByMap();
    ui->comboBox_findlist->clear();
    ui->comboBox_findlist->addItem("");
    ui->comboBox_findlist->addItems(getcomuselistbycurkeys());

}

void MainWindow::on_comboBox_editTextChanged(const QString &arg1)
{
    on_comboBox_currentIndexChanged(arg1);
}

void MainWindow::on_comboBox_findlist_currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_pushButton_clicked()
{
    ui->comboBox_findlist->setEditText("");
    ui->comboBox_findlist->setFocus();
}

void MainWindow::on_pushButton_push2box_clicked()
{
    QString execCmd = ui->textEdit->toPlainText();
    if(execCmd.simplified().length() == 0)
        return;
    TodoCmdExecList.append(execCmd);
    qDebug() << "Todo cmd exec list size:" << TodoCmdExecList.size();
}


void MainWindow::CheckTodoListTimerOut()
{
    if(TodoCmdExecList.size() == 0)
        return;
    static bool isdoing = FALSE;

    if(isdoing == TRUE)
        return;

    isdoing = TRUE;
End:
    foreach (QString str, TodoCmdExecList) {
        if(!ui->pushButton_connect->isEnabled())
        {
            Sleep(1000);
            continue;
        }
        qApp->processEvents();
        ui->statusBar->showMessage(QString::fromLocal8Bit("���������������:%1").arg(TodoCmdExecList.size()));
        checkoneitem_execcmd(str);
        qApp->processEvents();
        TodoCmdExecList.removeAt(0);
        if(TodoCmdExecList.size() > 0)
            goto End;
    }

    isdoing = FALSE;
    ui->statusBar->showMessage("");
}

void MainWindow::on_pushButton_delkey_clicked()
{
    if(ui->comboBox_findlist->currentText().simplified().length() == 0)
        return;

    QString delword = ui->comboBox_findlist->currentText();

    delKeyWord(delword  + KEY_DELETEWORD, TRUE);
}

QStringList MainWindow::getcomuselistbycurkeys()
{
    QString curkey = ui->comboBox->currentText();
    //    map_commonuselist
    QMapIterator<QString, QStringList> i(map_commonuselist);
    qDebug() << "com use list key:" << curkey ;

    QStringList findlist;
    quint8 found = FALSE;
    findlist.clear();
    while (i.hasNext()) {
        i.next();
        if(i.key() == curkey)
        {
            qDebug() << "com use list key,value:" << curkey << "," << i.value();
            found = TRUE;
            findlist = i.value();
            break;
        }
    }
    //    //û������ʱ��������
    //    if(found == FALSE)
    //    {
    //        QStringList nulllist;
    //        nulllist.clear();
    //        map_commonuselist.insert(curkey, nulllist);
    //        findlist = nulllist;
    //    }

    return findlist;
}


void MainWindow::insertcomuselistbycurkeys(QStringList list)
{
    QString curkey = ui->comboBox->currentText();
    map_commonuselist.insert(curkey, list);
}

void MainWindow::insertfindkeys2comuselist(QString findstr)
{
    QString curkey = ui->comboBox->currentText();
    QMapIterator<QString, QStringList> i(map_commonuselist);
    //    qDebug() << "com use list key:" << curkey ;

    QStringList findlist;
    quint8 found = FALSE;
    findlist.clear();
    while (i.hasNext()) {
        i.next();
        if(i.key() == curkey)
        {
            //            qDebug() << "com use list key,value:" << curkey << "," << i.value();
            found = TRUE;
            findlist = i.value();
            break;
        }
    }

    //û������ʱ��������
    if(found == FALSE)
    {
        QStringList nulllist;
        nulllist.clear();
        map_commonuselist.insert(curkey, nulllist);
        findlist = nulllist;
    }

    found = FALSE;
    foreach (QString instr, findlist) {
        if(instr == findstr)
        {
            found = TRUE;
            break;
        }
    }

    if(found == FALSE)
    {
        comuseitemlist = getcomuselistbycurkeys();
        comuseitemlist.append(findstr);
        comuseitemlist.sort();
        comuseitemlist.removeDuplicates();
        insertcomuselistbycurkeys(comuseitemlist);
    }
}



void MainWindow::printMapVar(QMap<QString, QStringList> &maps)
{
    qDebug() << "map var size:"<< maps.size();
    QMapIterator<QString, QStringList> i(maps);
    while (i.hasNext()) {
        i.next();
        qDebug() << "com use list key,value size:" << i.key() << "," << i.value().size();
        foreach (QString instr, i.value()) {
            qDebug() << "instr:" << instr;
        }

    }
}
void MainWindow::on_pushButton_searchengine_clicked(QString enginetext, quint8 cflag)
{
    quint8 covertflag = cflag;
    QString head = enginetext;
    QString cliptext = getclipboardtext();
    //    if(cliptext.simplified().isEmpty())
    //    {
    //        return;
    //    }
    //    m_labelOutput->setText(QString(byteArrayPercentEncoded));

    QString searchtext;
    //    QTextCodec * codecGB2312 = QTextCodec::codecForName("GB2312");
    if(!cliptext.simplified().isEmpty() && covertflag)
    {
        //        QByteArray byteArrayGB2312 = codecGB2312->fromUnicode(cliptext);
        //        QByteArray byteArrayGB2312 = cliptext.toLocal8Bit().toPercentEncoding();
        //        QByteArray byteArrayPercentEncoded = byteArrayGB2312.toPercentEncoding();
        QByteArray byteArrayPercentEncoded = cliptext.toLocal8Bit().toPercentEncoding();
        searchtext = head + byteArrayPercentEncoded;
    }
    else
    {
        searchtext = head + cliptext;
    }
    ui->textEdit->setText(searchtext);
    on_pushButton_connect_clicked_selftext(searchtext);

}
#define ENGINESEARCHTEXT(text,cflag)\
    do{\
    forceipaddrflag = true;\
    on_pushButton_searchengine_clicked( text, cflag);\
    forceipaddrflag = false;\
    }while(0)


void MainWindow::on_pushButton_baidu_clicked()
{
    ENGINESEARCHTEXT( "https://www.baidu.com/s?wd=", true);
}

void MainWindow::on_pushButton_biying_clicked()
{
    ENGINESEARCHTEXT( "https://cn.bing.com/search?q=", true);
}

void MainWindow::on_pushButton_360so_clicked()
{
    ENGINESEARCHTEXT( "https://www.so.com/s?ie=utf-8&fr=none&src=360sou_newhome&q=", true);
}
void MainWindow::on_pushButton_notepadpp_clicked()
{
    ENGINESEARCHTEXT( "cmd notepad++ ", false);
}


QString MainWindow::get_last_open_dir()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);  //QSettings�ܼ�¼һЩ�����е���Ϣ���´��ٴ�ʱ���Զ�ȡ����
    QString lastPath = setting.value("LastFilePath").toString();  //��ȡ�ϴεĴ�·��
    return lastPath;
}

QString MainWindow::set_last_open_dir(QString dir)
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);  //QSettings�ܼ�¼һЩ�����е���Ϣ���´��ٴ�ʱ���Զ�ȡ����
    QString lastPath = setting.value("LastFilePath").toString();  //��ȡ�ϴεĴ�·��
    setting.setValue("LastFilePath",dir);  //��¼·����QSetting�б���
    return lastPath;
}

void MainWindow::on_pushButton_saveresult_clicked()
{
    QString savetext = getrighttext();
    if (cursavefilepath.isEmpty())
    {
        return;
    }
    QString fileName = cursavefilepath;
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(savetext.toLocal8Bit());
    }
    file.close();
}

void MainWindow::on_pushButton_saveresult2_another_clicked()
{
    QString savetext = getrighttext();
    QString fileName = QFileDialog::getSaveFileName(this
                                                    ,tr("Save File")
                                                    ,get_last_open_dir()
                                                    ,tr("Text Files (*.txt);;csv Files (*.csv);;Html Files (*.html;*.htm);;*.*")
                                                    );
    if (fileName.isNull())
    {
        return;
    }
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(savetext.toLocal8Bit());
    file.close();
}




void MainWindow::on_pushButton_openfile2result_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    get_last_open_dir(),
                                                    tr("Text Files (*.txt);;csv Files (*.csv);;Html Files (*.html;*.htm);;*.*"),
                                                    0);
    if(!fileName.isEmpty())
    {
        set_last_open_dir(fileName);  //��¼·����QSetting�б���
        myWatcher.removePath(get_last_open_dir());
        myWatcher.addPath(fileName);
        qDebug() << "fileName:" << fileName;
    }
    else
    {
        return;
    }

    update_selectedfile(fileName);
}

QString MainWindow::getrighttext()
{
    return ui->textEdit->toPlainText();
}
void MainWindow::setrighttext(QString text)
{
    ui->textEdit->clear();
    ui->textEdit->setText(text);
    ui->textEdit->update();
}

/**
  ** �ж��ļ��Ƿ�ΪUTF8����
  **/
bool MainWindow::IsUTF8(const void* pBuffer, long size)
{
    bool IsUTF8 = true;
    unsigned char* start = (unsigned char*)pBuffer;
    unsigned char* end = (unsigned char*)pBuffer + size;
    while (start < end)
    {
        if (*start < 0x80) // (10000000): ֵС��0x80��ΪASCII�ַ�
        {
            start++;
        }
        else if (*start < (0xC0)) // (11000000): ֵ����0x80��0xC0֮���Ϊ��ЧUTF-8�ַ�
        {
            IsUTF8 = false;
            break;
        }
        else if (*start < (0xE0)) // (11100000): �˷�Χ��Ϊ2�ֽ�UTF-8�ַ�
        {
            if (start >= end - 1)
            {
                break;
            }

            if ((start[1] & (0xC0)) != 0x80)
            {
                IsUTF8 = false;
                break;
            }

            start += 2;
        }
        else if (*start < (0xF0)) // (11110000): �˷�Χ��Ϊ3�ֽ�UTF-8�ַ�
        {
            if (start >= end - 2)
            {
                break;
            }

            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
            {
                IsUTF8 = false;
                break;
            }

            start += 3;
        }
        else
        {
            IsUTF8 = false;
            break;
        }
    }

    return IsUTF8;
}
bool MainWindow::IsUTF8File(const char* pFileName)
{
    FILE *f = NULL;
    f = fopen(pFileName, "rb");
    if (NULL == f)
    {
        return false;
    }

    fseek(f, 0, SEEK_END);
    long lSize = ftell(f);
    fseek(f, 0, SEEK_SET);  //��rewind(f);

    char *pBuff = new char[lSize + 1];
    memset(pBuff, 0, lSize + 1);
    fread(pBuff, lSize, 1, f);
    fclose(f);

    bool bIsUTF8 = IsUTF8(pBuff, lSize);
    delete []pBuff;
    pBuff = NULL;

    return bIsUTF8;
}


void MainWindow::on_checkBox_echoswitch_toggled(bool checked)
{

}

void MainWindow::on_checkBox_echoswitch_clicked()
{

}

void MainWindow::on_checkBox_fileautoload_clicked()
{

}

void MainWindow::procDirChanged(const QString &path)
{
    if(!ui->checkBox_fileautoload->isChecked())
    {
        qDebug() << "none select, no deal!!";
        return;
    }
    qDebug() << "file changed!!";

    update_selectedfile(path);
    //    ShowTipsInfo(str_china(""));
    ShowTipsInfoWithShowTime(str_china("�����ļ�:") + path, 1000);

//    oldpath = path;

}

QString MainWindow::GetCorrectUnicode(const QByteArray &ba)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode( ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
    {
        text = QTextCodec::codecForName( "GBK" )->toUnicode(ba);
    }
    else
    {
        text = ba;
    }

    return text;
}

void MainWindow::update_selectedfile(QString fileName)
{
    qDebug() << "update file cotent " << fileName;
//    QFileInfo info(fileName);
    QFile file(fileName);
    QString text;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString strAll=stream.readAll();
        if(IsUTF8File(fileName.toLocal8Bit().data()))
        {
            text = QString::fromUtf8(strAll.toLocal8Bit());
        }
        else
        {
            text = QString::fromLocal8Bit(strAll.toLocal8Bit());
        }
        qDebug() << "text:" << text;
        setrighttext(text);
        cursavefilepath = fileName;
        ui->label->setText(orglabeltext + "  " + cursavefilepath);
    }

    file.close();

//    QStringList files = myWatcher.files();
//    foreach (QString file, files) {
//        qDebug() << "watch file:" << file;
//    }
}

void MainWindow::on_checkBox_tree_toggled(bool checked)
{
    if(ui->checkBox_tree->isChecked())
    {
        ui->treeView_ut->show();
    }
    else
    {
        ui->treeView_ut->hide();
    }
}


void MainWindow::LoadDirTree()
{
    qDebug() << "LoadDirTree()";
    QFileDialog::Options options = QFileDialog::ShowDirsOnly;
//    options |= QFileDialog::HideNameFilterDetails;
//    options |= QFileDialog::DontResolveSymlinks;
//    options |= QFileDialog::DontUseSheet;
//    options |= QFileDialog::DontConfirmOverwrite;
//    options |= QFileDialog::DontUseNativeDialog;
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          tr("Open Directory"),
                                                          curmodeldir,
                                                          options);
    if (!directory.isEmpty())
    {
        //        ui->textEdit->setText(directory);
        initDirTree(directory);
    }
}


void MainWindow::initDirTree(QString directory = DEFAULT_LOADTREE_DIR)
{
    if(directory.isEmpty())
    {
        directory = DEFAULT_LOADTREE_DIR;
    }

    if(model)
    {
        delete model;
        model = NULL;
    }

    model = new QFileSystemModel;
    model->setRootPath(directory);
    ui->treeView_ut->setModel(model);
    ui->treeView_ut->setRootIndex(model->index(directory));
    connect(ui->treeView_ut, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(procDoubleClickTree(QModelIndex)));

    curmodeldir = directory;
    ui->statusBar->showMessage(QString("Loading ") + QString::fromLocal8Bit(directory.toLocal8Bit()));

    //    �����ϸ�鿴�ͻᷢ�֣����ǵ���ͼ���������ܵ����ͷ��Ϊ�ˣ����ǿ���ʹ��������룺
    ui->treeView_ut->header()->setStretchLastSection(true);
    ui->treeView_ut->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->treeView_ut->header()->setSortIndicatorShown(true);
#if QT_VERSION >= 0x050000
    ui->treeView_ut->header()->setSectionsClickable(true);
#else
    ui->treeView_ut->header()->setClickable(true);
#endif

}

void MainWindow::procDoubleClickTree(QModelIndex currentIndex)
{
    qDebug() << "QModelIndex :" << currentIndex;
    QString fileName =  QFileSystemModel().filePath(currentIndex);
    if(!QFileSystemModel().isDir(currentIndex))
    {
        myWatcher.removePath(get_last_open_dir());
        set_last_open_dir(fileName);  //��¼·����QSetting�б���
        myWatcher.addPath(fileName);
        qDebug() << "fileName:" << fileName;
        update_selectedfile(fileName);
    }

}

void MainWindow::TreeMkdir()
{
    QModelIndex index = ui->treeView_ut->currentIndex();
    if (!index.isValid()) {
        qDebug() << "index invalid";
        return;
    }
    QString dirName = QInputDialog::getText(this,
                                            tr("Create Directory"),
                                            tr("Directory name"));
    if (!dirName.isEmpty()) {
        qDebug() << "dir ok";
        if (!model->mkdir(index, dirName).isValid()) {
            qDebug() << "if ok";
            QMessageBox::information(this,
                                     tr("Create Directory"),
                                     tr("Failed to create the directory"));
        }
        else
        {
            qDebug() << "else ng";
        }
    }
}

void MainWindow::TreeRm()
{
    QModelIndex index = ui->treeView_ut->currentIndex();
    if (!index.isValid()) {
        return;
    }
    bool ok;
    QString tips = tr("Are you Sure Remove %1 ?").arg(model->fileName(index));
    int ret = QMessageBox::critical(NULL, "critical", tips,
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::Yes);
    if(ret != QMessageBox::Yes)
    {
        return;
    }

    if (model->fileInfo(index).isDir()) {
        ok = model->rmdir(index);
    } else {
        ok = model->remove(index);
    }
    if (!ok) {
        QMessageBox::information(this,
                                 tr("Remove"),
                                 tr("Failed to remove %1").arg(model->fileName(index)));
    }
}



void MainWindow::on_checkBox_dict_realtime_toggled(bool checked)
{
    if(checked)
    {
//        cliptexttimer->start(1000);
    }
    else
    {
//        cliptexttimer->stop();
    }

}

QString MainWindow::getClipboard_Str(void)
{
    return QApplication::clipboard()->text();
}

void MainWindow::CheckDictTimerOut()
{
    if(!ui->checkBox_dict_realtime->isChecked())
    {
        return;
    }
    static QString o;
    QString n = getClipboard_Str();
    if(n != o)
    {
        o = n;
        on_pushButton_biying_clicked();
    }

}
