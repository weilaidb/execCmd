#include "processbarx.h"
#include <QMessageBox>

QProgressDialog *Cprocessbarx::pInstance = NULL;
Cprocessbarx::Cprocessbarx()
{
//    if(NULL == pInstance)
//    {
//        new QProgressDialog( QString::fromUtf8("正在保存......"), QString::fromUtf8("取消"),0,100,this);
//    }
}


void Cprocessbarx::setvalue(quint64 val, quint64 max)
{
    if(NULL == pInstance)
    {
        return;
    }
    pInstance->setValue(val/max);
}

int Cprocessbarx::wascanceld(QString tips)
{
    if(pInstance->wasCanceled())
    {
        QMessageBox::warning(NULL,QString::fromUtf8("提示"), (tips));
        return 0;
    }
    return -1;
}

void Cprocessbarx::show()
{
    pInstance->show();
}

void Cprocessbarx::getInstance()

{
    if(NULL == pInstance)
    {
        new QProgressDialog( QString::fromUtf8("正在......"), QString::fromUtf8("取消"),0,100,NULL);
        show();
    }
}
