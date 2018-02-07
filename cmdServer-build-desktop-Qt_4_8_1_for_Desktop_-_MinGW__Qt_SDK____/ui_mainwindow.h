/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Feb 7 18:55:08 2018
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QGroupBox *horizontalGroupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_connect;
    QLineEdit *lineEdit_sendnum;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_collect;
    QListWidget *listWidget_cmdlist;
    QSplitter *splitter_3;
    QSplitter *splitter_4;
    QLabel *label_3;
    QCheckBox *checkBox_autosend;
    QComboBox *comboBox_findlist;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(742, 397);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(51, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(251, 31));

        horizontalLayout_2->addWidget(lineEdit);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(131, 16));
        label->setFont(font);

        verticalLayout->addWidget(label);

        textEdit = new QTextEdit(layoutWidget1);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        textEdit->setFont(font1);
        textEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 227, 255);"));

        verticalLayout->addWidget(textEdit);

        splitter->addWidget(layoutWidget1);
        splitter_2->addWidget(splitter);
        horizontalGroupBox = new QGroupBox(splitter_2);
        horizontalGroupBox->setObjectName(QString::fromUtf8("horizontalGroupBox"));
        horizontalLayout = new QHBoxLayout(horizontalGroupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_connect = new QPushButton(horizontalGroupBox);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        pushButton_connect->setMinimumSize(QSize(91, 41));
        pushButton_connect->setMaximumSize(QSize(107, 41));

        horizontalLayout->addWidget(pushButton_connect);

        lineEdit_sendnum = new QLineEdit(horizontalGroupBox);
        lineEdit_sendnum->setObjectName(QString::fromUtf8("lineEdit_sendnum"));
        lineEdit_sendnum->setMinimumSize(QSize(10, 10));
        lineEdit_sendnum->setMaximumSize(QSize(25, 41));
        lineEdit_sendnum->setMaxLength(300);

        horizontalLayout->addWidget(lineEdit_sendnum);

        pushButton_clear = new QPushButton(horizontalGroupBox);
        pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
        pushButton_clear->setMinimumSize(QSize(91, 41));
        pushButton_clear->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_clear);

        pushButton_collect = new QPushButton(horizontalGroupBox);
        pushButton_collect->setObjectName(QString::fromUtf8("pushButton_collect"));
        pushButton_collect->setMinimumSize(QSize(91, 41));
        pushButton_collect->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_collect);

        splitter_2->addWidget(horizontalGroupBox);

        gridLayout->addWidget(splitter_2, 0, 0, 2, 1);

        listWidget_cmdlist = new QListWidget(centralWidget);
        listWidget_cmdlist->setObjectName(QString::fromUtf8("listWidget_cmdlist"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        listWidget_cmdlist->setFont(font2);
        listWidget_cmdlist->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(listWidget_cmdlist, 1, 1, 1, 1);

        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        splitter_4 = new QSplitter(splitter_3);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        label_3 = new QLabel(splitter_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setFont(font);
        splitter_4->addWidget(label_3);
        checkBox_autosend = new QCheckBox(splitter_4);
        checkBox_autosend->setObjectName(QString::fromUtf8("checkBox_autosend"));
        checkBox_autosend->setFont(font);
        splitter_4->addWidget(checkBox_autosend);
        comboBox_findlist = new QComboBox(splitter_4);
        comboBox_findlist->setObjectName(QString::fromUtf8("comboBox_findlist"));
        comboBox_findlist->setEditable(true);
        splitter_4->addWidget(comboBox_findlist);
        splitter_3->addWidget(splitter_4);

        gridLayout->addWidget(splitter_3, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\221\275\344\273\244\345\256\242\346\210\267\347\253\257", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\211\247\350\241\214\345\221\275\344\273\244", 0, QApplication::UnicodeUTF8));
        pushButton_connect->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_sendnum->setToolTip(QApplication::translate("MainWindow", "\346\211\247\350\241\214\346\225\260\351\207\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_clear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
        pushButton_collect->setText(QApplication::translate("MainWindow", "\346\224\266\350\227\217", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "\345\221\275\344\273\244\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        checkBox_autosend->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
