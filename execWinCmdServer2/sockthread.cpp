﻿#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <windows.h>
#include <QHostAddress>
#include <QTextCodec>
#include "sockthread.h"
#include "getstrdata.h"

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

    initData();

    plog = new wlog();
//    clientConnection = /*cltConnection*/;
}

//    QObject::connect(clientConnection,SIGNAL(readyRead()),
//                     this,SLOT(updateReadMsgProgress()));
//    QObject::connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),
//                     this,SLOT(displayErr(QAbstractSocket::SocketError)));


void sockthread::initData()
{
    //读数据统计
    TotalReadBytes = 0;
    bytesReceived  = 0;
    bytesNeedRecv  = 0;

    //写数据统计
    TotalBytes   = 0;
    byteWritten  = 0;
    bytesToWrite = 0;

    printFlag = 0;
}

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
    if(clientConnection)
    {
        emit emitErrInfo(QString("%1:%2").arg("errinfo").arg(clientConnection->errorString()),
                         (void *)this);
//        QMessageBox::information(NULL,str_china("网络"),
//                                 str_china("产生如下错误： %1")
//                                 .arg(clientConnection->errorString()));
    }
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
统一规定:发送侧使用UTF-8字符编码
接收侧使用gb18030解码，解析出来才是汉字
* Time        : 2017-05-28
============================================*/
void sockthread::updateReadMsgProgress()
{
    QTextCodec *textc_gbk = QTextCodec::codecForName("gb18030");
    /**
      ** 统一使用默认使用的字符编码为utf-8
      ** 发送的数据也是utf-8
      **/
    QTextCodec *textc_utf8 = QTextCodec::codecForName("UTF-8");

    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_6);

    static bool recvdone = READ_DONE;

    if(bytesReceived <= sizeof(qint64)*1  && (recvdone == READ_DONE)){
        if((clientConnection->bytesAvailable() >= sizeof(qint64)*1)){
            in>>TotalReadBytes;
//            bytesReceived += sizeof(qint64)*1;
            qDebug() << inBlock.data();
            PrintStrData("inBlock", (BYTE *)inBlock.data(), inBlock.size());
            inBlock.resize(0);
            recvdone = READING;
            qDebug() << "TotalReadBytes:" << TotalReadBytes;
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
        QString  bigmsg;
        bigmsg.clear();
        PrintStrData("afinBlock", (BYTE *)inBlock.data(), inBlock.size());
        plog->SaveDefault(inBlock);
        if(inBlock.at(0) != '\0')
        {
//            bigmsg = QString::fromUtf8(inBlock); //不知道为什么，数据里有许多其它内容，前4个字节有数据为\0的信息
            bigmsg = textc_utf8->toUnicode(inBlock); //不知道为什么，数据里有许多其它内容，前4个字节有数据为\0的信息
//            bigmsg = textc_gbk->toUnicode(inBlock); //不知道为什么，数据里有许多其它内容，前4个字节有数据为\0的信息
            qDebug() << "get from head";
        }
        else
        {
//            bigmsg = textc_gbk->toUnicode(inBlock.mid(4));
            bigmsg = textc_utf8->toUnicode(inBlock.mid(4));
            qDebug() << "get from head 4byte";
        }
        //入库
//        readfromremote(bigmsg);
        emit emitMsgDoneSignal(bigmsg, (void *)this);
        qDebug() << "read msg size:" << bigmsg.size();

        qDebug() << "read msg:" << bigmsg;
//        qDebug() << "read msg:" << bigmsg.toLocal8Bit();
//        qDebug() << "read msg:" << bigmsg.toUtf8();
//        qDebug() << "read msg:" << bigmsg.toAscii();
//        qDebug() << "read msg:" << bigmsg.toLatin1();
//        qDebug() << "read msg:" << bigmsg.toAscii();


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
    QTextCodec *textc_gbk = QTextCodec::codecForName("gb18030");
    /**
      ** 统一使用默认使用的字符编码为utf-8
      ** 发送的数据也是utf-8
      **/
    QTextCodec *textc_utf8 = QTextCodec::codecForName("UTF-8");

    qDebug() << "write msg:" << msgs;

    outBlock.resize(0); //用于暂存我们要发送的数据
    QDataStream out(&outBlock, QIODevice::WriteOnly);
    out.resetStatus();
    out.setVersion(QDataStream::Qt_4_6);

    //设置数据流的版本，客户端和服务器端使用的版本要相同
    //先占用8个字节表示发送后面body的长度
    out<<(quint64) 0;
    printByteArray("push header zero", outBlock);

    const char * tempd = msgs.toStdString().c_str();
//    qDebug() << "tempd:" << tempd << ",len:" << strlen(tempd);
    //使用writeRawData消除产生的多余4个字节问题
    out.writeRawData(tempd, strlen(tempd));
//    out<< textc_utf8->fromUnicode(msgs); //<<QString或 << QByteArray会额外产生4个字节
    printByteArray("push body", outBlock);

    //从头索引，计算发送数据的长度（BODY），不带HEADER占用的8字节，重新填充HEADER占用的8字节
    out.device()->seek(0);
    out<<(quint64)(outBlock.size()-sizeof(quint64));//计算发送数据的大小
    //打印outBlock的内容，使用16进制
    printByteArray("calc header size", outBlock);


    TotalBytes = outBlock.size();

    //打印outBlock的内容，使用16进制
    printByteArray("last send msg", outBlock);


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
    qDebug() << (QString("got new client(%1)")
                 .arg(clientConnection->peerAddress().toString()));

    QObject::connect(clientConnection,SIGNAL(readyRead()),
                     this,SLOT(updateReadMsgProgress()));
//    QObject::connect(clientConnection,SIGNAL(bytesWritten(qint64)),
//                     this,SLOT(updateWriteClientProgress(qint64)));
    QObject::connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),
                     this,SLOT(displayErr(QAbstractSocket::SocketError)));


}

QTcpSocket *sockthread::getSocketConnect()
{
    return clientConnection;
}

void sockthread::closeSocketConnect()
{
    quint8 loop = 0;
    while ( loop < 10)
    {
//        Sleep(1000);
        loop++;
    }
//    qDebug() << "socket closed!!";
    initData();
    clientConnection->close();
    clientConnection->deleteLater();
    clientConnection = NULL;
}

void sockthread::printByteArray(const char *pTips, QByteArray bytearray)
{
    if(0 == printFlag)
    {
        return;
    }
    unsigned int len = bytearray.size();
    unsigned int dwLp = 0;

    qDebug() << "------Print Byte Array[" << pTips << "], size:" << len;

    for(dwLp = 0; dwLp < len; dwLp++)
    {
        qDebug("0x%02x ",(unsigned char *)bytearray.data()[dwLp]);
    }
}
