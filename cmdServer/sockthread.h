#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QAbstractSocket>


#if 1
//编码汉字
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif



enum
{
    READ_DONE,
    READING,
    READ_OTHER,

    WRITE_DONE,
    WRITEING,
    WRITE_OTHER
};


class sockthread : public QThread
{
    Q_OBJECT
public:
    explicit sockthread(QObject *parent = 0);
    void setSocketConnect(QTcpSocket *cltConnet);
    void sendmsg(QString msgs);

signals:
    void emitMsgDoneSignal(QString, void *);
    void emitErrInfo(QString, void *);

public slots:
    void displayErr(QAbstractSocket::SocketError socketError);
    void updateReadMsgProgress();
    void updateWriteClientProgress(qint64 numBytes);

protected:
    void run();

protected:

    QTcpSocket *clientConnection;

    //写数据统计
    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;
//    QString fileName;
//    QFile *localFile;
    QByteArray outBlock;
    QByteArray outBlockFile;//文件字节序列

    //读数据统计
//    qint64 TotalReadBytes;
//    qint64 byteReadden;
//    qint64 bytesToRead;
//    QByteArray outReadBlock;

    qint64  TotalReadBytes;
    qint64  bytesReceived;
//    qint64  fileNameSize;
    qint64  bytesNeedRecv;
    QByteArray inBlock;
};

#endif // SOCKTHREAD_H
