#include "cgetfilecheckxxsum.h"
#include <QProgressBar>
#include <QProgressDialog>
#include "processbarx.h"


CGetFileCheckxxSum::CGetFileCheckxxSum()
{
}


QByteArray CGetFileCheckxxSum::getFileMd5(QString filePath, QString &outstring)
{
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error." << filePath;
        outstring = filePath + "[file open error.]";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    QProgressDialog *process = new QProgressDialog();
    process->setRange(0, 100);
    process->show();

    while (1)
    {

        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }

        process->setValue((bytesWritten * 100) /totalBytes);
        if(process->wasCanceled())
        {
            break;
        }
    }

    process->hide();
    delete process;

    localFile.close();
    QByteArray md5 = ch.result();
    return md5;
}



QByteArray CGetFileCheckxxSum::getFileSha1(QString filePath, QString &outstring)
{
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error." << filePath;
        outstring = filePath + "[file open error.]";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Sha1);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    QProgressDialog *process = new QProgressDialog();
    process->setRange(0, 100);
    process->show();

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
        process->setValue((bytesWritten * 100) /totalBytes);
        if(process->wasCanceled())
        {
            break;
        }
    }

    process->hide();
    delete process;

    localFile.close();
    QByteArray sha1 = ch.result();
    return sha1;
}


QString CGetFileCheckxxSum::fileMd5(const QString &sourceFilePath)
{

    QFile sourceFile(sourceFilePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}


