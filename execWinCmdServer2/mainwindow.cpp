#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <windows.h>

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

    InitServer("127.0.0.1", 99999);

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
    qDebug() << (QString("read clt msg:%1").arg(cltmsg));

    //python.exe
    LPCSTR exepath = "explorer.exe";
    LPCSTR filepath = cltmsg.toAscii().data();
    LPCSTR filepath2 = QString::fromUtf8(filepath).toLocal8Bit().data();

    ShellExecuteA(NULL,"open", exepath,filepath2,NULL,SW_SHOWNORMAL);
    ui->statusBar->showMessage(QString("%1 %2").arg(exepath).arg(QString::fromUtf8(filepath)));
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
