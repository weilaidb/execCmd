#ifndef CGETFILECHECKXXSUM_H
#define CGETFILECHECKXXSUM_H
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QDebug>


class CGetFileCheckxxSum
{
public:
    CGetFileCheckxxSum();

    static QByteArray getFileMd5(QString filePath, QString &outstring);
    static QString fileMd5(const QString &sourceFilePath);

    static QByteArray getFileSha1(QString filePath, QString &outstring);
};

#endif // CGETFILECHECKXXSUM_H
