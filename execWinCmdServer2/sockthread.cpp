#include "sockthread.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <windows.h>


#if 1
//编码汉字
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif


/*============================================
* FuncName    : sockthread::sockthread
* Description :
* @parent     :
* Author      :
* Time        : 2017-05-28
============================================*/
sockthread::sockthread(QObject *parent) :
    QThread(parent),clientConnection(NULL)
{

    //读数据统计
    TotalReadBytes = 0;
    bytesReceived  = 0;
    bytesNeedRecv  = 0;

    //写数据统计
    TotalBytes   = 0;
    byteWritten  = 0;
    bytesToWrite = 0;

//    clientConnection = /*cltConnection*/;
}

//    QObject::connect(clientConnection,SIGNAL(readyRead()),
//                     this,SLOT(updateReadMsgProgress()));
//    QObject::connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),
//                     this,SLOT(displayErr(QAbstractSocket::SocketError)));


/*============================================
* FuncName    : sockthread::run
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::run()
{
    while(1)
    {
//        qDebug() << "emit signal.....";
//        emit emitMsgBoxSignal();
        Sleep(1000);
    }
}


/*============================================
* FuncName    : sockthread::displayErr
* Description :
* @socketError  :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::displayErr(QAbstractSocket::SocketError socketError)
{
//    if(socketError == QTcpSocket::RemoteHostClosedError)
//        return;
    emit emitErrInfo(QString("%1:%2").arg("errinfo").arg(clientConnection->errorString()),
                     (void *)this);
//    QMessageBox::information(NULL,str_china("网络"),
//                             str_china("产生如下错误： %1")
//                             .arg(clientConnection->errorString()));
//    tcpserver->close();
//    tcpserver = NULL;

    if(clientConnection)
    {
        clientConnection->close();
        clientConnection = NULL;
    }
#ifdef SHOWCURSOR
    QApplication::restoreOverrideCursor();
#endif
}

/*============================================
* FuncName    : sockthread::updateReadMsgProgress
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::updateReadMsgProgress()
{
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_6);

    static bool recvdone = READ_DONE;

    if(bytesReceived <= sizeof(qint64)*1  && (recvdone == READ_DONE)){
        if((clientConnection->bytesAvailable() >= sizeof(qint64)*1)){
            in>>TotalReadBytes;
//            bytesReceived += sizeof(qint64)*1;
            inBlock.resize(0);
            recvdone = READING;
        }
    }


    if (bytesReceived < TotalReadBytes){
        /* 实际需要接收的字节数 */
        bytesNeedRecv = TotalReadBytes - bytesReceived;
        bytesReceived += clientConnection->bytesAvailable();

        if(bytesReceived >= TotalReadBytes){
            inBlock.append(clientConnection->read(bytesNeedRecv));
            bytesReceived = TotalReadBytes;
        }else{
            inBlock.append(clientConnection->readAll());
        }

        qDebug() << "bytesReceived:"<< bytesReceived;
        qDebug() << "TotalReadBytes   :"<< TotalReadBytes;
    }

    if (bytesReceived == TotalReadBytes) {
        QString  bigmsg = inBlock.mid(4); //不知道为什么，数据里有许多其它内容，前4个字节有数据为\0的信息
        //入库
//        readfromremote(bigmsg);
        emit emitMsgDoneSignal(bigmsg, (void *)this);
        qDebug() << "read msg size:" << bigmsg.size();

        qDebug() << "read msg:" << bigmsg.toLocal8Bit();

        TotalReadBytes = 0;
        bytesReceived = 0;
//        fileNameSize = 0;
        bytesNeedRecv = 0;
        inBlock.resize(0);

        recvdone = READ_DONE;
    }


}




/*============================================
* FuncName    : autoCCode::updateWriteClientProgress
* Description :
* @numBytes   :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::updateWriteClientProgress(qint64 numBytes)
{
    static bool writeflag = WRITE_DONE;
    qDebug() << "numBytes:--------->>"<<numBytes;
    byteWritten += (int)numBytes;
    if(bytesToWrite > 0)
    {
        qDebug() <<"-->:outBlock size:" << outBlock.size();

        bytesToWrite -= (int)clientConnection->write(outBlock);
        qDebug() <<"-->:bytesToWrite size:" << bytesToWrite;
        writeflag = WRITEING;
    }
    else
    {
        qDebug() << "-->: send msg done!!";
        TotalBytes = 0;
        byteWritten = 0;
        writeflag = WRITE_DONE;
    }
}

void sockthread::sendmsg(QString msgs)
{
    qDebug() << "write msg:" << msgs;

    outBlock.resize(0); //用于暂存我们要发送的数据
    QDataStream out(&outBlock, QIODevice::WriteOnly);
    out.resetStatus();
    out.setVersion(QDataStream::Qt_4_6);

    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out<<(quint64) 0;
    //要发送的数据放到out
    out<< msgs.toUtf8().data(); //必须是转换后的字符
//    out << "hello world";
    out.device()->seek(0);
    out<<(quint64)(outBlock.size()-sizeof(quint64));//计算发送数据的大小

    TotalBytes = outBlock.size();

    bytesToWrite = TotalBytes - clientConnection->write(outBlock);//将名称发出后，剩余图片大小
    qDebug() << "TotalBytes:" << TotalBytes;
    qDebug() << "bytesToWrite:" << bytesToWrite;
}

/*============================================
* FuncName    : sockthread::setSocketConnect
* Description :
* @cltConnet  :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::setSocketConnect(QTcpSocket *cltConnet)
{
    clientConnection = cltConnet;
    QObject::connect(clientConnection,SIGNAL(readyRead()),
                     this,SLOT(updateReadMsgProgress()));
    QObject::connect(clientConnection,SIGNAL(bytesWritten(qint64)),
                     this,SLOT(updateWriteClientProgress(qint64)));
    QObject::connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),
                     this,SLOT(displayErr(QAbstractSocket::SocketError)));


}

void sockthread::closeSocketConnect()
{
    clientConnection->close();
    clientConnection->deleteLater();
}

