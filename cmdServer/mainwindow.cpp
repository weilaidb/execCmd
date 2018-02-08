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

#define BINDPORT (99999)
#define CMD_HELLO "hello word"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pthreadsock(NULL),
    uselistTimer(NULL)
{
    ui->setupUi(this);

//splitte分割比例
    ui->splitter->setStretchFactor(0,6);
    ui->splitter->setStretchFactor(1,4);

    ReadHistorySettings();

    ui->listWidget_cmdlist->clear();
    ui->listWidget_cmdlist->addItems(show_cmdlist);

    ui->comboBox_findlist->addItems(commonuselist);
    updateListWidgetColor();
    publicSets();
    PopMenu();
    autosendstr.clear();
    ui->lineEdit_sendnum->setValidator(new QIntValidator(0, 100, this)); ;
    ui->lineEdit_sendnum->hide();
    ui->statusBar->showMessage(QString::fromLocal8Bit(version));

    uselistTimer = new QTimer();
    uselistTimer->start(4000);
    connect(uselistTimer, SIGNAL(timeout()), this, SLOT(procUseListTimerOut()));
//    ui->comboBox_findlist->setShortcutEnabled();
//    ui->comboBox_findlist->setShortcutAutoRepeat();
//     ui->udpSendButton->setShortcut(tr("Alt+F"));

     QShortcut *findShortCut = new QShortcut(this);
     findShortCut->setKey(tr("Ctrl+F"));
     connect(findShortCut, SIGNAL(activated()),this,SLOT(procFindShortCut()));

     QShortcut *findClearShortCut = new QShortcut(this);
     findClearShortCut->setKey(tr("Ctrl+D"));
     connect(findClearShortCut, SIGNAL(activated()),this,SLOT(procClearShortCut()));


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::publicSets()
{

    ui->listWidget_cmdlist->setMouseTracking(true);
    connect(ui->listWidget_cmdlist, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(procDoubleClickItem(QListWidgetItem *)));
    connect(ui->listWidget_cmdlist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(procDoubleClickItem(QListWidgetItem *)));
    connect(ui->listWidget_cmdlist, SIGNAL(itemEntered(QListWidgetItem*)),this, SLOT(procEnterItem(QListWidgetItem*)));
    connect(ui->comboBox_findlist, SIGNAL(editTextChanged(QString)), this, SLOT(procFindList(QString)));

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
    ui->lineEdit->setText(m_settings.value("LineEditIP").toString());
    show_cmdlist.clear();
    show_cmdlist = m_settings.value("listWidget_cmdlist").toStringList();
    commonuselist.clear();
    commonuselist = m_settings.value("comboBox_findlist").toStringList();

    ui->lineEdit->setText(m_settings.value("LineEditIP").toString());
    ui->checkBox_autosend->setChecked(m_settings.value("checkBox_autosend").toBool());
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
    m_settings.setValue("LineEditIP",ui->lineEdit->text());
//    show_cmdlist.clear();
//    int count = ui->listWidget_cmdlist->count();
//    int loop = 0;
//    for(loop = 0; loop < count;loop++)
//    {
//        cmdlist << ui->listWidget_cmdlist->item(loop)->text();
//    }
    m_settings.setValue("listWidget_cmdlist",show_cmdlist);
    m_settings.setValue("comboBox_findlist",commonuselist);
    m_settings.setValue("checkBox_autosend",ui->checkBox_autosend->isChecked());
    m_settings.setValue("Cmdserver", this->saveGeometry());

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
    event->accept();
}


int MainWindow::CheckIPAddr(QString ipaddr)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!regExp.exactMatch(ipaddr))
    {
        ShowTipsInfo(str_china("提示, ip地址错误"));
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

void MainWindow::on_pushButton_connect_clicked()
{
    QString ipaddr = ui->lineEdit->text();
    if(0 != CheckIPAddr(ipaddr))
    {
        return;
    }
    ui->pushButton_connect->setEnabled(false);

    socket = new QTcpSocket();

    newConnect(ipaddr);


    qDebug() << "on connecting";
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
    ShowTipsInfoWithShowTime(str_china("正在连接中..."), 2000);
    qApp->processEvents();

    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。
    socket->connectToHost(ipaddr, BINDPORT);
    connect(socket, SIGNAL(connected()), this, SLOT(hellosocket()));




    qDebug() <<"socket err:" << socket->errorString();
    qDebug() <<"remote ip addr:" << ipaddr;


    //当返回false时可以调用error来确定无法连接的原因
    if(!socket->waitForConnected(3000))
    {
        ShowTipsInfoWithShowTime(str_china("连接超时，请检查服务器地址是否正确"), 2000);
        ui->pushButton_connect->setEnabled(true);
        //        出错后，广播查找sever
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
    ShowTipsInfo(QString::fromLocal8Bit("连接服务器成功！！"));

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

void MainWindow::updateListWidgetColor()
{
    int count = ui->listWidget_cmdlist->count();
    int loop = 0;
    for(loop = 0; loop < count;loop++)
    {
        if(loop % 2 )
            ui->listWidget_cmdlist->item(loop)->setForeground(QColor(Qt::red));
        else
            ui->listWidget_cmdlist->item(loop)->setForeground(QColor(Qt::blue));

//        ui->listWidget_cmdlist->item(loop)->setSizeHint(QSize(0,0));
    }
//    ui->listWidget_cmdlist->setShown(true);
}

void MainWindow::on_pushButton_collect_clicked()
{
    if(ui->textEdit->toPlainText().length() == 0)
    {
        return;
    }
    QString currenttext = ui->textEdit->toPlainText();
//    currenttext.replace("\n"," && ");

//    cmdlist.clear();
//    int count = ui->listWidget_cmdlist->count();
//    int loop = 0;
//    for(loop = 0; loop < count;loop++)
//    {
//        cmdlist << ui->listWidget_cmdlist->item(loop)->text();
//    }

    foreach (QString str, show_cmdlist) {
        if(str == currenttext)
            return;
    }

    show_cmdlist << currenttext;
    ui->listWidget_cmdlist->clear();
    ui->listWidget_cmdlist->addItems(show_cmdlist);
    ui->listWidget_cmdlist->sortItems();
//    ui->listWidget_cmdlist->setTextElideMode(Qt::ElideRight);
    updateListWidgetColor();
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::procDoubleClickItem(QListWidgetItem * item)
{
    if(ui->checkBox_autosend->isChecked())
    {
        autosendstr = item->text();
        ui->textEdit->setText(item->text());
        on_connecting_sendstr("");

        return;
    }
    qDebug() << item->text();
    ui->textEdit->setText(item->text());
}

void MainWindow::procEnterItem(QListWidgetItem* item)
{
    qDebug() << "enter " << item->text();
    setToolTip(item->text());
}

void MainWindow::procFindList(QString findstr)
{
    if(ui->comboBox_findlist->currentText().simplified().length() == 0)
    {
        ui->listWidget_cmdlist->clear();
        ui->listWidget_cmdlist->addItems(show_cmdlist);
        ui->listWidget_cmdlist->sortItems();
//        ui->listWidget_cmdlist->setTextElideMode(Qt::ElideRight);
        updateListWidgetColor();
        return;
    }
    searchlist.clear();
    QStringList splitlist = findstr.split(QRegExp("\\s"));
    int size = splitlist.size();
    int count = 0;
    foreach (QString str, show_cmdlist) {
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
//    int count = ui->listWidget_cmdlist->count();
//    int loop = 0;
//    for(loop = 0; loop < count;loop++)
//    {
//        if(ui->listWidget_cmdlist->item(loop)->text().contains(findstr))
//            searchlist << ui->listWidget_cmdlist->item(loop)->text();
//    }

    ui->listWidget_cmdlist->clear();
    ui->listWidget_cmdlist->addItems(searchlist);
    ui->listWidget_cmdlist->sortItems();
    ui->listWidget_cmdlist->setTextElideMode(Qt::ElideRight);
    updateListWidgetColor();

}


/* 右键菜单 */
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
    menu->addAction(Act_DelItem); //添加菜单项1
    //    menu->addAction(Act_Normal); //添加菜单项2
    menu->exec(cur.pos()); //关联到光标
}

void MainWindow::PopMenu()
{
    /* 右键菜单 */
    Act_DelItem = new QAction(tr("DelItem"), this);
    connect(Act_DelItem, SIGNAL(triggered()), this, SLOT(DelItem()));
}


void MainWindow::DelItem()
{
    show_cmdlist.removeOne(ui->listWidget_cmdlist->currentItem()->text());
    ui->listWidget_cmdlist->takeItem(ui->listWidget_cmdlist->currentRow());
    ui->listWidget_cmdlist->sortItems();
}

void MainWindow::procUseListTimerOut()
{
    static QString oldstr = ui->comboBox_findlist->currentText().simplified();
    QString curstr = ui->comboBox_findlist->currentText().simplified();
    qDebug() << "oldstr:" << oldstr;
    qDebug() << "curstr:" << curstr;
    if(oldstr != curstr)
    {
        oldstr = curstr;
        return;
    }
    if(ui->listWidget_cmdlist->count() == 0 )
    {
        return;
    }
    qDebug() << "!!!procUseListTimerOut add uselist:" << curstr;
    commonuselist.append(curstr);
    commonuselist.sort();
    commonuselist.removeDuplicates();
    ui->comboBox_findlist->clear();
    ui->comboBox_findlist->addItems(commonuselist);
    ui->comboBox_findlist->setEditText(curstr);
    ui->comboBox_findlist->update();
}

void MainWindow::procFindShortCut()
{
    qDebug() << "procFindShortCut";
//    ui->comboBox_findlist->setEditText("");
    ui->comboBox_findlist->setFocus();
}
void MainWindow::procClearShortCut()
{
    qDebug() << "procClearShortCut";
    ui->comboBox_findlist->setEditText("");
}


