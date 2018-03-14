#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <sockthread.h>
#include <QListWidget>
#include <QTimer>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void publicSets();

protected:
    void ReadHistorySettings();
    void WriteCurrentSettings();
    int CheckIPAddr(QString ipaddr);
public:
    void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void PopMenu();
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *keyevt);
    void keyReleaseEvent(QKeyEvent * keyevt);
    void UpdateShowCmdListWidget(QStringList list);
    void UpdateShowCmdListWidgetByMap();
    QStringList GetCurrentMapValue();

private:
    bool bCtrlKeyPressed;

private:
    QAction * Act_DelItem;
    QAction * Act_Normal;

private:
    QMap<QString, QStringList> map_showcmd;
    QStringList show_cmdlist;
    QStringList searchlist;
    QString autosendstr;
    QStringList commonuselist;
    QTimer *uselistTimer;
    QTimer *savetimer;
private slots:
    void on_connecting_sendstr(QString sendstr);
    void on_pushButton_connect_clicked();

    void newConnect(QString ipaddr);
    void hellosocket();
    void procErrMsg(QString errmsg,void*);

    void readfromremote(QString msg,void*);

    void on_pushButton_collect_clicked();

    void on_pushButton_clear_clicked();

    void procDoubleClickItem(QListWidgetItem *item);

    void DelItem();

    void updateListWidgetColor();

    void procFindList(QString findstr);

    void procEnterItem(QListWidgetItem * item);

    void procUseListTimerOut();

    void procSaveTimerOut();

    QString getMapKey();

    void procFindShortCut();

    void procClearShortCut();

    void procSendCmdShortCut();

    void procitemSelectionChanged();

    void procComBoxIpList(QString ipaddr);
    void on_pushButton_paste_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_editTextChanged(const QString &arg1);

    void on_comboBox_findlist_currentIndexChanged(const QString &arg1);

private:
    QTcpSocket *socket;
    QString message;  //存放从服务器接收到的字符串

    sockthread *pthreadsock;

private:
    Ui::MainWindow *ui;

    QStringList ComBoxIPList;
    QString curkey;

    QMap<QString, QStringList> mapIpAndContent;
};

#endif // MAINWINDOW_H
