#include "msgtips.h"
#include <QtGui>
#include "msgtipsautoshut.h"

msgtips::msgtips()
{
}

void ShowTipsInfo(QString s)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL);
    tipsinfo->SetTipsInfo(s);
    //��������ͼ�꣬�޹��������ö���ʾ
    tipsinfo->setWindowFlags( Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    tipsinfo->show();

}

void ShowTipsInfoWithShowTime(QString s, quint32 ultimeout)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL, ultimeout);
    tipsinfo->SetTipsInfo(s);
    //��������ͼ�꣬�޹��������ö���ʾ
    tipsinfo->setWindowFlags( Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    tipsinfo->show();

}
