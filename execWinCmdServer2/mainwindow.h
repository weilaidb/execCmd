#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "msgtips.h"
#include <QRegExp>
#include "sockthread.h"
#include <QUdpSocket>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    int InitServer( QString ipaddr, quint16 listenport);
    void closeallclientsocket();
    void showversion();

private slots:
    void procClientMessage();
    void readfromremote(QString cltmsg, void * pthread);
    LPCSTR singstep(const char *org, bool isCmd, QString single, HINSTANCE &ret);
    char *convertQString2buf(QString str);

private:
    Ui::MainWindow *ui;

private:
    QTcpServer *tcpServer;
    QTcpSocket *pCltSock;

    QString logs;

    QList<sockthread *> socklist;
    QList<sockthread *>::iterator it_sklst;
    //写数据统计
    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;

    QByteArray outBlock; //用于暂存我们要发送的数据

    QTcpSocket *clientConnection;


private:
    void createActions();
    void createTrayIcon();
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
private:
#if 1
    QSystemTrayIcon *trayIcon;
    char *msg;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *savetofileAction;
    QAction *quitAction;
    QAction *startAction;
    QAction *stopAction;
    QMenu *trayIconMenu;
#endif

};

#endif // MAINWINDOW_H
