#ifndef MSGTIPSAUTOSHUT_H
#define MSGTIPSAUTOSHUT_H

#include <QWidget>
#include <QtGui>
//#include <QtWidgets>

class MsgTipsAutoShut : public QWidget
{
    Q_OBJECT
public:
    MsgTipsAutoShut();
    explicit MsgTipsAutoShut(QWidget *parent = 0);
    MsgTipsAutoShut(QWidget *parent,quint32 ultimeout);
    void SetTipsInfo(QString s);
    void WidgetXYsetDesktop_center(QWidget *widget);

signals:
    
public slots:
    void closeEvent(QCloseEvent *event);
private:
    QString str;
    QTimer *timer;
    QLabel *label;
    quint32 m_ultimeout;

};

#endif // MSGTIPSAUTOSHUT_H
