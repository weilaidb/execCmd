#ifndef PROCESSBARX_H
#define PROCESSBARX_H
#include <QString>
#include <QProgressBar>
#include <QProgressDialog>

class Cprocessbarx:public QProgressDialog
{
public:
    Cprocessbarx();
    static void setvalue(quint64 val, quint64 max);
    static int wascanceld(QString tips);
    static void show();
    static void getInstance();

private:
    static QProgressDialog *pInstance;
};

#endif // PROCESSBARX_H
