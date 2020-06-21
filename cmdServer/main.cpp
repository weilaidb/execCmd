#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *textc_gbk = QTextCodec::codecForName("gb18030");
    /**
      ** 统一使用默认使用的字符编码为utf-8
      ** 发送的数据也是utf-8
      **/
    QTextCodec *textc_utf8 = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForCStrings(textc_utf8);
    QTextCodec::setCodecForTr(textc_utf8);
    QTextCodec::setCodecForLocale(textc_utf8);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
