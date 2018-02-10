#include "sockthread.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <windows.h>


#if 1
//���뺺��
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

    //������ͳ��
    TotalReadBytes = 0;
    bytesReceived  = 0;
    bytesNeedRecv  = 0;

    //д����ͳ��
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
//    QMessageBox::information(NULL,str_china("����"),
//                             str_china("�������´��� %1")
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
        /* ʵ����Ҫ���յ��ֽ��� */
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
        QString  bigmsg = inBlock.mid(4); //��֪��Ϊʲô��������������������ݣ�ǰ4���ֽ�������Ϊ\0����Ϣ
        //���
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

    outBlock.resize(0); //�����ݴ�����Ҫ���͵�����
    QDataStream out(&outBlock, QIODevice::WriteOnly);
    out.resetStatus();
    out.setVersion(QDataStream::Qt_4_6);

    //�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ
    out<<(quint64) 0;
    //Ҫ���͵����ݷŵ�out
    out<< msgs.toUtf8().data(); //������ת������ַ�
//    out << "hello world";
    out.device()->seek(0);
    out<<(quint64)(outBlock.size()-sizeof(quint64));//���㷢�����ݵĴ�С

    TotalBytes = outBlock.size();

    bytesToWrite = TotalBytes - clientConnection->write(outBlock);//�����Ʒ�����ʣ��ͼƬ��С
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

