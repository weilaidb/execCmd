#include <QFile>
#include <QString>
#include <QDebug>
#include "wlog.h"


wlog::wlog()
{
    defaultfilename = "wdefault.log";
}

void wlog::SaveContent(QString filename, QString content)
{
    QFile file(filename);
    if(file.exists())
    {
        file.open(QIODevice::Append);
    }
    else
    {
        file.open(QIODevice::WriteOnly);
    }
    file.write(content.toUtf8() + "\n");
    file.close();
}

void wlog::SaveDefault(QString content)
{
    qDebug() << "save:" << content;
    SaveContent(defaultfilename, content);
}
