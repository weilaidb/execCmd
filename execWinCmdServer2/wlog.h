#ifndef WLOG_H
#define WLOG_H

class wlog
{
public:
    wlog();
    void SaveContent(QString filename, QString content);
    void SaveDefault(QString content);

private:
    QString defaultfilename;
};

#endif // WLOG_H
