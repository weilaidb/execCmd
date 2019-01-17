#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <sockthread.h>
#include <QListWidget>
#include <QTimer>
#include <QMap>
#include <QMutex>
#include <QSettings>

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
    void Var2Map(QSettings &sets, QString envkey, QMap<QString, QStringList> &outmap);
    QMap<QString, QVariant> Map2Var(QMap<QString, QStringList> &inmap);
public:
    void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void PopMenu();
    void SearchMenu();
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *keyevt);
    void keyReleaseEvent(QKeyEvent * keyevt);
    void UpdateShowCmdListWidget(QString findstr, QStringList list);
    void UpdateShowCmdListWidgetByMap();
    QStringList GetCurrentMapValue();

private:
    bool bCtrlKeyPressed;

private:
    QAction * Act_DelItem;
    QAction * Act_Normal;
    QAction * Act_searchbaidu;
    QAction * Act_searchbiying;
    QAction * Act_search360so;
    QAction * Act_notepadpp;
    QAction * Act_saveresult;
    QAction * Act_openfile2result;

private:
    QMap<QString, QStringList> map_showcmd; //IP地址 -> IP地址存储的内容
    QStringList show_cmdlist;
    QStringList searchlist;
//    QMap<QString, QStringList> searchlist;
    QString oneshowres;
    QString autosendstr;
    QMap<QString, QStringList> map_commonuselist; //IP地址 -> 关键词表
    QStringList comuseitemlist; //关键词表
    QTimer *uselistTimer;
    QTimer *savetimer;
private slots:
    void on_connecting_sendstr(QString sendstr);
    void on_pushButton_connect_clicked();
    void on_pushButton_connect_clicked_selftext(QString text);

    void checkoneitem_execcmd(QString text);

    void newConnect(QString ipaddr);
    void hellosocket();
    void procErrMsg(QString errmsg,void*);

    void readfromremote(QString msg,void*);

    void on_pushButton_collect_clicked();

    void on_pushButton_clear_clicked();

    void procDoubleClickItem(QListWidgetItem *item);

    void procClickItem(QListWidgetItem *item);

    void DelItem();

    void updateListWidgetColor();

    void procFindList(QString findstr);

    void procActivatedFindList(QString findstr);

    void procEnterItem(QListWidgetItem * item);

    void procUseListTimerOut();

    bool delKeyWord(QString curstr, bool setnull);

    void procSaveTimerOut();

    QString getMapKey();

    void procFindShortCut();

    void procClearShortCut();

    void procSendCmdShortCut();

    QString filterInvalidText(QString orgt);
    void filterText(QString text);

    void procitemSelectionChanged();

    void procComBoxIpList(QString ipaddr);
    void on_pushButton_paste_clicked();
    QString getclipboardtext();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_editTextChanged(const QString &arg1);

    void on_comboBox_findlist_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_push2box_clicked();

    void CheckTodoListTimerOut();

    void on_pushButton_delkey_clicked();

    QStringList getcomuselistbycurkeys();

    void insertcomuselistbycurkeys(QStringList list);

    void insertfindkeys2comuselist(QString findstr);

    void printMapVar(QMap<QString, QStringList> &maps);

    void on_pushButton_searchengine_clicked(QString text, quint8 cflag);

    void on_pushButton_baidu_clicked();

    void on_pushButton_biying_clicked();

    void on_pushButton_360so_clicked();

    void on_pushButton_notepadpp_clicked();

    void on_pushButton_saveresult_clicked();

    void on_pushButton_openfile2result_clicked();

    QString getrighttext();

    void setrighttext(QString text);

    bool IsUTF8File(const char* pFileName);

    bool IsUTF8(const void* pBuffer, long size);

    void on_checkBox_echoswitch_toggled(bool checked);

    QString get_last_open_dir();
    QString set_last_open_dir(QString dir);

private:
    QTcpSocket *socket;
    QString message;  //存放从服务器接收到的字符串

    sockthread *pthreadsock;

    QMutex mutex_search;

    bool forceipaddrflag;

private:
    Ui::MainWindow *ui;

    QStringList ComBoxIPList;
    QString curkey;

    QMap<QString, QStringList> mapIpAndContent;

private:
    QStringList TodoCmdExecList;//待处理的命令个数
    QTimer *exectimer;
};

#endif // MAINWINDOW_H
