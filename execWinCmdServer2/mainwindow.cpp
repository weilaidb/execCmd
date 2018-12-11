#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <windows.h>
#include <version.h>
#include <QRegExp>

#define MAX_LENGTH (20480)
#define BINDPORT (9999)

char* szLogin = new char[MAX_LENGTH];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpServer(NULL)
{
    ui->setupUi(this);

    //写数据统计
    TotalBytes   = 0;
    byteWritten  = 0;
    bytesToWrite = 0;

//    InitServer("127.0.0.1", 9999);
//    InitServer("192.168.59.12", BINDPORT);
    InitServer("0", BINDPORT);

    //qt 最小化到托盘
#if 1
    //fileexist("images/good1.ico");
    QIcon icon3 = QIcon(":/app1.ico");

    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/app1.ico"), QSize(), QIcon::Normal, QIcon::On);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon3);
    trayIcon->setToolTip("win execuate machine");
    createActions();
    createTrayIcon();
    trayIcon->show();

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
#endif

    showversion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::InitServer( QString ipaddr, quint16 listenport)
{
    if(NULL != tcpServer)
    {
        closeallclientsocket();
        tcpServer->disconnect(this);
        tcpServer->close();
        //        tcpServer->deleteLater();
        tcpServer = NULL;
    }

    tcpServer = new QTcpServer(this);
    //if(!tcpServer->listen(QHostAddress::LocalHost,6666))
    //    if(!tcpServer->listen(QHostAddress("192.168.1.100"),6666))
    if(!tcpServer->listen(QHostAddress(ipaddr),listenport))
//    if(!tcpServer->listen(QHostAddress::Any,listenport))
    {  //监听本地主机的6666端口，如果出错就输出错误信息，并关闭

        qDebug() << tcpServer->errorString();
        //        close();
        return -1;
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(procClientMessage()));
    return 0;
}

void MainWindow::closeallclientsocket()
{
    qDebug() << (QString("close all clt socket"));


    for(it_sklst = socklist.begin(); it_sklst != socklist.end(); it_sklst++)
    {
        sockthread * inthread = (sockthread * )*it_sklst;
        inthread->closeSocketConnect();
    }
    socklist.clear();
}

void MainWindow::procClientMessage()
{
    clientConnection = tcpServer->nextPendingConnection();

    qDebug() << "-->>client socket:" << clientConnection;
    qDebug() << "-->>client socket address:" << clientConnection->peerAddress();
    sockthread *pthreadsock = new sockthread(this);
    pthreadsock->setSocketConnect(clientConnection);
    QObject::connect(pthreadsock,SIGNAL(emitMsgDoneSignal(QString,void*)),
                     this,SLOT(readfromremote(QString, void *)));
    QObject::connect(pthreadsock,SIGNAL(emitErrInfo(QString,void*)),
                     this,SLOT(dealclienterror(QString, void *)));


    pthreadsock->start();
    socklist.push_back(pthreadsock);
    qDebug() << "-->>socklist size:" << socklist.size();


    ui->statusBar->showMessage("proc new client message...");
    qDebug() << (QString("got new client(%1)")
                 .arg(clientConnection->peerAddress().toString()));
}




void MainWindow::readfromremote(QString cltmsg, void * pthread)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
//    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale()); //设置GBK到本地
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    qDebug() << "clt msg size:" << cltmsg.size();
//    qDebug() << "clt         :" << cltmsg;
//    qDebug() << "clt         :" << tr(cltmsg.toUtf8());
    qDebug() << "clt         :" << tr(cltmsg.toAscii()); //ok
//    qDebug() << "clt         :" << tr(cltmsg.toLocal8Bit());
    qDebug() << "clt         :" << tr(cltmsg.toLatin1()); //ok
//    qDebug() << (QString::fromUtf8("read clt msg:%1").arg(cltmsg.toUtf8().data()));
//    qDebug() << (QString::fromLocal8Bit("read clt msg:%1").arg(cltmsg.toAscii().data()));
//    qDebug() << (QString::fromAscii("read clt msg:%1").arg(cltmsg.toLocal8Bit().data()));
//    qDebug() << (QString::fromLatin1("read clt msg:%1").arg(cltmsg.toUtf8().data()));
//    qDebug() << (QString::fromUtf8("read clt msg:%1").arg(cltmsg.toAscii().data()));
//    qDebug() << (QString::fromUtf8("read clt msg:%1").arg(cltmsg.toLocal8Bit().data()));
////    qDebug() << (QString::fromUtf8("read clt msg:%1").arg(cltmsg.toAscii().data()));


    //python.exe
    //    LPCSTR exepath = "explorer.exe";
    LPCSTR exepath = "";
//    LPCSTR filepath = cltmsg.toAscii().data();

    QStringList splitdata = cltmsg.split('\n');
    QString showtext;

    foreach (QString single, splitdata) {
        if(single.simplified().length() == 0)
            continue;
        bool isCmd = FALSE;
        if(single.contains("cmd") && single.simplified() != "cmd")
        {
            isCmd = TRUE;
        }
        single = tr(single.toAscii().data());
        qDebug() << "single         :" << single;
        LPCSTR filepath = (single.toAscii().data());
        HINSTANCE ret;
        quint32 cret = 0;
        LPCSTR resstring = singstep(filepath, isCmd, single, ret);
        cret = (quint32)ret;
        qDebug() << "ret :" << ret;
        qDebug() << "cret:" << cret;
        qDebug() << "resstring:" << resstring;
        showtext += QString("ret:%1[%2] %3 \n")
                            .arg(cret)
                            .arg(cret < 32 ? "err" : "ok")
                            .arg(QString::fromLocal8Bit(resstring).toUtf8().data())
                            ;
    }
    ui->statusBar->showMessage(tr(showtext.toLatin1()));
    ui->textEdit_receive->setText(tr(cltmsg.toLatin1()));


//    LPCSTR filepath2 = QString::fromUtf8(filepath).toLocal8Bit().simplified().data();
//    qDebug() << "filepath2:" << QString::fromUtf8(filepath).toLocal8Bit().simplified().data();
//    if(isCmd)
//    {
//        cltmsg = "/c " + cltmsg.replace("cmd", "");
//        filepath = cltmsg.toAscii().data();
//        filepath2 = QString::fromUtf8(filepath).toLocal8Bit().data();
//        ShellExecuteA(NULL, "open", "cmd", filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
//    }
//    else
//    {
//        //    ShellExecuteA(NULL,"open", exepath,filepath2,NULL,SW_SHOWNORMAL);
//        ShellExecuteA(NULL,"open", filepath2,NULL,NULL,SW_SHOWMAXIMIZED);
//    }




    sockthread *threadsock = (sockthread *)pthread;
    threadsock->closeSocketConnect();


}




void MainWindow::createActions()
{
#if 1
    restoreAction = new QAction(tr("normal (&R)"), this);

    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    //    savetofileAction= new QAction(tr("savetofile (&S)"), this);
    //    connect(savetofileAction, SIGNAL(triggered()), this, SLOT(savetofile()));

    //    startAction= new QAction(tr("start (&S)"), this);
    //    connect(startAction, SIGNAL(triggered()), this, SLOT(startautocollect()));

    //    stopAction= new QAction(tr("stop (&S)"), this);
    //    connect(stopAction, SIGNAL(triggered()), this, SLOT(stopautocollect()));

    minimizeAction = new QAction(tr("min (&I)"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("max (&X)"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    quitAction = new QAction(tr("quit (&Q)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
#endif
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    //    trayIconMenu->addAction(savetofileAction);
    //    trayIconMenu->addAction(startAction);
    //    trayIconMenu->addAction(stopAction);
#if 0
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
#endif

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
}



//最小化到托盘----
void MainWindow::changeEvent(QEvent *e)
{
#if 1
    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
    {
        QTimer::singleShot(100, this, SLOT(close()));
    }
#endif
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
    qDebug() << "MainWindow closeEvent";
    //    WriteCurrentSettings();
    //    saveResource();
    //    event->accept();
    event->ignore();
    this->hide();
}

void MainWindow::showversion()
{
    ui->statusBar->showMessage(QString("version %1").arg(VERSION));
}



char*  MainWindow::convertQString2buf(QString single)
{
    LPCSTR filepath2 = NULL;

    memset(szLogin, 0, MAX_LENGTH);
    QByteArray ba111 = single.toLocal8Bit(); // strUser是QString，外部传来的数据。
    char* temp111 = ba111.data();
    strcpy(szLogin, temp111);
    //    然后强行转换char*到LPCWSTR：
    filepath2 = (LPCSTR)szLogin;
    fprintf(stdout, "temp111  :%s\n", temp111);
    fprintf(stdout, "szLogin  :%s\n", szLogin);
    fprintf(stdout, "filepath2:%s\n", filepath2);
    fflush(stdout);
    return szLogin;
}

LPCSTR MainWindow::singstep(const char *org,bool isCmd,QString single, HINSTANCE &ret)
{
    LPCSTR filepath = LPCSTR(org);
    LPCSTR filepath2 = NULL;

    if(szLogin == NULL)
        return filepath2;

    filepath2 = convertQString2buf(single);


////    LPCSTR filepath2 = QString::fromAscii(filepath).toAscii().simplified().data();
////    qDebug() <<"iscmd:" << isCmd << "filepath2:" << QString::fromUtf8(filepath).toLocal8Bit().simplified().data();
//    qDebug() <<"iscmd:" << isCmd ;
//    qDebug() << "filepath2:" << QString::fromAscii(filepath).toAscii().simplified().data();
//    qDebug() << "filepath2:" << QString::fromUtf8(filepath).toAscii().simplified().data();
//    qDebug() << "filepath2:" << QString::fromUtf8(filepath).toAscii().simplified().data();
//    qDebug() << "filepath2:" << QString::fromAscii(filepath);
//    qDebug() << "filepath2:" << QString::fromUtf8(filepath);
//    qDebug() << "filepath2:" << QString::fromLatin1(filepath);
//    qDebug() << "filepath2:" << QString::fromLocal8Bit(filepath);
    qDebug() << "filepath2:" << (filepath);
    qDebug() << "single   :" << (single);


    try
    {
        // 这里的程序代码完成真正复杂的计算工作，这些代码在执行过程中
        // 有可能抛出DataType1、DataType2和DataType3类型的异常对象。

        if(isCmd)
        {
    //        single = "/c "/* + single.replace("cmd", 3, "", 3)*/;
            QString strTmp = "";
//            int idx = 0;

            strTmp = single;
            //only replace the first find cmd
            strTmp.replace(QRegExp("^cmd\\s+"), "");
//            idx = strTmp.indexOf("cmd ", 0);
//            if (-1 != idx)
//            {
//                strTmp.replace(idx, 4, "");
//    //            m_Button->setText(strTmp);
//            }
            single = QString("/C %1").arg(strTmp);
    //        single = strTmp;
            qDebug() << "cmd order:" << single;
            qDebug() << "strTmp   :" << strTmp;


            filepath = single.toAscii().data();
            qDebug() << "filepath :" << filepath;
//            qDebug() << "filepath[0] :" << filepath[0];
//            qDebug() << "filepath[1] :" << filepath[1];
//            //跳过前导空格
//            while(*filepath == ' ')
//                filepath++;

//            filepath2 = QString::fromUtf8(filepath).toLocal8Bit().data();
//            filepath2 = QString::fromUtf8(filepath).toUtf8().data();
            filepath2 = convertQString2buf(single);
//            //跳过前导空格
//            while(*filepath2 == ' ')
//                filepath2++;

            qDebug() << "filepath2 last:" << filepath2;
            ret = ShellExecuteA(NULL, "open", "cmd", filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
    //        HINSTANCE ret = ShellExecuteA(NULL, "open", "C:\windows\system32\cmd.exe", filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
    //        HINSTANCE ret = ShellExecuteA(NULL, "open", NULL, filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
    //        SW_MINIMIZE
        }
        else
        {
            //    ShellExecuteA(NULL,"open", exepath,filepath2,NULL,SW_SHOWNORMAL);
            ret = ShellExecuteA(NULL,"open", filepath2,NULL,NULL,SW_SHOWMAXIMIZED);
        }
        qDebug() << "ret:" << ret;

    }
    catch(QString & d1)
    {
        qDebug() << " filepaths too long ?";
    }


    return filepath2;
}

