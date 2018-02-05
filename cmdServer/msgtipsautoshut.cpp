#include "msgtipsautoshut.h"

#if 1
//���뺺��
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif


MsgTipsAutoShut::MsgTipsAutoShut()
{

}

MsgTipsAutoShut::MsgTipsAutoShut(QWidget *parent) :
    QWidget(parent),
    m_ultimeout(2000)
{
    setWindowFlags(Qt::FramelessWindowHint);
    label = new QLabel(this);
    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()), this,SLOT(close()));
//    resize(200,80);

//    setLayoutDirection(Qt::LayoutDirectionAuto);

}

MsgTipsAutoShut::MsgTipsAutoShut(QWidget *parent,quint32 ultimeout) :
    QWidget(parent),
    m_ultimeout(ultimeout)
{
    setWindowFlags(Qt::FramelessWindowHint);
    label = new QLabel(this);
    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()), this,SLOT(close()));
//    resize(200,80);

//    setLayoutDirection(Qt::LayoutDirectionAuto);

}


void MsgTipsAutoShut::SetTipsInfo(QString s)
{
    str = s;
    label->setText(QString::fromLocal8Bit(str.toLocal8Bit()));
    label->setStyleSheet("QLabel{font: bold italic large \"Times New Roman\";font-size:25px;color:rgb(55,100,255);height:50px;border:4px solid rgb(155,200,33);background-color: rgba(0,0,0,30);border-radius:15px;selection-color:pink}");
    label->setWordWrap(false);
    label->adjustSize(); //�Զ�������С
    label->setAlignment(Qt::AlignCenter);
    timer->start(m_ultimeout);
//    setLayoutDirection(Qt::LayoutDirectionAuto);
    WidgetXYsetDesktop_center(this);
//    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height())/2);
}

void MsgTipsAutoShut::closeEvent(QCloseEvent *event)
{
    event = event;
//    QMessageBox::StandardButton button;
//    button = QMessageBox::question(this, str_china("�˳�����"),
//        QString(str_china("���棺������һ���������������У��Ƿ���������˳�?")),
//        QMessageBox::Yes | QMessageBox::No);

//    if (button == QMessageBox::No) {
//        event->ignore();  //�����˳��źţ������������
//    }
//    else if (button == QMessageBox::Yes) {
//        event->accept();  //�����˳��źţ������˳�
//    }
}

void MsgTipsAutoShut::WidgetXYsetDesktop_center(QWidget *widget)
{
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
//    UNUSEDVAR(wd);
    widget->move((wd - width()/2)/2,(ht-height()/2)/2);
}


