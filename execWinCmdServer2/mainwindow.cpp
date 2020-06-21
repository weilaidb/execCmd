﻿#include "mainwindow.h"
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

}

void MainWindow::dealclienterror(QString cltmsg, void * pthread)
{
    qDebug() << "dealclienterror:" << cltmsg.size();
    sockthread *p = (sockthread *)pthread;
    p->closeSocketConnect();

}


void MainWindow::readfromremote(QString cltmsg, void * pthread)
{
    qDebug() << "clt msg size:" << cltmsg.size();
//    qDebug() << "clt toAscii :" << tr(cltmsg.toAscii()); //ok
//    qDebug() << "clt toLatin1:" << tr(cltmsg.toLatin1()); //ng
    qDebug() << "clt org     :" << cltmsg; //ok

    QStringList splitdata = cltmsg.split('\n');
    QString showtext;

    foreach (QString single, splitdata) {
        if(single.simplified().length() == 0)
            continue;
        bool isCmd = FALSE;
        //去除开头和结尾空白
        single = single.trimmed();
        if(single.contains("cmd") && single.simplified() != "cmd")
        {
            isCmd = TRUE;
        }
        qDebug() << "single trimmed     :" << single;
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
                            .arg(showshellexecuteresult(cret))
                            .arg(single)
                            ;
    }
    ui->statusBar->showMessage((showtext));
    ui->textEdit_receive->setText(cltmsg);

    sockthread *threadsock = (sockthread *)pthread;
    //不写
//    threadsock->getSocketConnect()->write((showtext.toLatin1()));
//    threadsock->emitMsgDoneSignal();
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
//    QByteArray ba111 = single.toLocal8Bit(); // strUser是QString，外部传来的数据。
    QByteArray ba111 = single.toAscii(); // strUser是QString，外部传来的数据。
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
    LPCSTR filepath2 = NULL;

    if(szLogin == NULL)
        return filepath2;
    qDebug() << "singstep single   :" << (single);

    try
    {
        // 这里的程序代码完成真正复杂的计算工作，这些代码在执行过程中
        // 有可能抛出DataType1、DataType2和DataType3类型的异常对象。
        if(isCmd)
        {
            QString strTmp = "";
            strTmp = single;
            //only replace the first find cmd
            strTmp.replace(QRegExp("^cmd\\s+"), "");
            single = QString("/C %1").arg(strTmp);
            qDebug() << "cmd order:" << single;
            qDebug() << "strTmp   :" << strTmp;
            filepath2 = convertQString2buf(single);
            if(ui->checkBox_hidecmdblack->isChecked())
            {
                ret = ShellExecuteA(NULL, "open", "cmd", filepath2, NULL, SW_HIDE);
            }
            else
            {
                ret = ShellExecuteA(NULL, "open", "cmd", filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
            }
//            ret = ShellExecuteA(NULL, "open", "cmd", filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
//            ret = ShellExecuteA(NULL, "open", "cmd", filepath2, NULL, SW_HIDE);
    //        HINSTANCE ret = ShellExecuteA(NULL, "open", "C:\windows\system32\cmd.exe", filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
    //        HINSTANCE ret = ShellExecuteA(NULL, "open", NULL, filepath2, NULL, SW_SHOWNORMAL | SW_NORMAL | SW_SHOW);
    //        SW_MINIMIZE
        }
        else
        {
            //    ShellExecuteA(NULL,"open", exepath,filepath2,NULL,SW_SHOWNORMAL);
            ret = ShellExecuteA(NULL,"open", filepath2,NULL,NULL,SW_SHOWMAXIMIZED);
        }
        qDebug() << "ret:" << ret << ", " << showshellexecuteresult((quint32)ret);


    }
    catch(QString & d1)
    {
        qDebug() << " filepaths too long ?";
    }


    return filepath2;
}


char * MainWindow::showshellexecuteresult(quint32 ret)
{
//    返回值大于32表示执行成功
//    返回值小于32表示执行错误
//    返回值可能的错误有: = 0 {内存不足}
//    ERROR_FILE_NOT_FOUND = 2; {文件名错误}
//    ERROR_PATH_NOT_FOUND = 3; {路径名错误}
//    ERROR_BAD_FORMAT = 11; {EXE 文件无效}
//    SE_ERR_SHARE = 26; {发生共享错误}
//    SE_ERR_ASSOCINCOMPLETE = 27; {文件名不完全或无效}
//    SE_ERR_DDETIMEOUT = 28; {超时}
//    SE_ERR_DDEFAIL = 29; {DDE 事务失败}
//    SE_ERR_DDEBUSY = 30; {正在处理其他 DDE 事务而不能完成该 DDE 事务}
//    SE_ERR_NOASSOC = 31; {没有相关联的应用程序}


//    #define SHELLEXECUTEA_RET_2STR(num)\

    switch((quint32)ret)
    {
    case 2:
        return "ERROR_FILE_NOT_FOUND";
        break;
    case 3:
        return "ERROR_PATH_NOT_FOUND";
        break;
    case 11:
        return "ERROR_BAD_FORMAT";
        break;
    case 26:
        return "SE_ERR_SHARE";
        break;
    case 27:
        return "SE_ERR_ASSOCINCOMPLETE";
        break;
    case 28:
        return "SE_ERR_DDETIMEOUT";
        break;
    case 29:
        return "SE_ERR_DDEFAIL";
        break;
    case 30:
        return "SE_ERR_DDEBUSY";
        break;
    case 31:
        return "SE_ERR_NOASSOC";
        break;
    default:
        break;
    }

    if((int)ret > 32)
    {
        return "ok";
    }
    else
    {
        return "err";
    }

}
