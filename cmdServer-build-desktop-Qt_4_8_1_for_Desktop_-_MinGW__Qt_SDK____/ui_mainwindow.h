/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jul 23 23:04:36 2019
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
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_3;
    QLabel *label_3;
    QCheckBox *checkBox_autosend;
    QPushButton *pushButton_delkey;
    QCheckBox *checkBox_treeopen;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBox;
    QVBoxLayout *verticalLayout;
    QGroupBox *horizontalGroupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_connect;
    QPushButton *pushButton_push2box;
    QPushButton *pushButton_paste;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_collect;
    QPushButton *pushButton_baidu;
    QPushButton *pushButton_biying;
    QPushButton *pushButton_360so;
    QCheckBox *checkBox_forcelocalhost;
    QCheckBox *checkBox_dict_realtime;
    QCheckBox *checkBox_fileautoload;
    QCheckBox *checkBox_watchhttp;
    QCheckBox *checkBox_tree;
    QCheckBox *checkBox_openfirstitem;
    QLineEdit *lineEdit_sendnum;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSplitter *splitter;
    QTextEdit *textEdit;
    QTextEdit *textEdit_cmdresult;
    QSplitter *splitter_4;
    QComboBox *comboBox_findlist;
    QPushButton *pushButton;
    QSplitter *splitter_2;
    QTreeView *treeView_ut;
    QListWidget *listWidget_cmdlist;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1330, 500);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(splitter_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setMaximumSize(QSize(16777215, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        label_3->setFont(font);
        splitter_3->addWidget(label_3);
        checkBox_autosend = new QCheckBox(splitter_3);
        checkBox_autosend->setObjectName(QString::fromUtf8("checkBox_autosend"));
        checkBox_autosend->setFont(font);
        splitter_3->addWidget(checkBox_autosend);
        pushButton_delkey = new QPushButton(splitter_3);
        pushButton_delkey->setObjectName(QString::fromUtf8("pushButton_delkey"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_delkey->sizePolicy().hasHeightForWidth());
        pushButton_delkey->setSizePolicy(sizePolicy);
        pushButton_delkey->setMinimumSize(QSize(20, 0));
        splitter_3->addWidget(pushButton_delkey);
        checkBox_treeopen = new QCheckBox(splitter_3);
        checkBox_treeopen->setObjectName(QString::fromUtf8("checkBox_treeopen"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(12);
        checkBox_treeopen->setFont(font1);
        splitter_3->addWidget(checkBox_treeopen);

        gridLayout->addWidget(splitter_3, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(60, 16));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(251, 31));
        comboBox->setEditable(true);

        horizontalLayout_2->addWidget(comboBox);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalGroupBox = new QGroupBox(centralWidget);
        horizontalGroupBox->setObjectName(QString::fromUtf8("horizontalGroupBox"));
        horizontalLayout = new QHBoxLayout(horizontalGroupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_connect = new QPushButton(horizontalGroupBox);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        pushButton_connect->setMinimumSize(QSize(60, 41));
        pushButton_connect->setMaximumSize(QSize(107, 41));

        horizontalLayout->addWidget(pushButton_connect);

        pushButton_push2box = new QPushButton(horizontalGroupBox);
        pushButton_push2box->setObjectName(QString::fromUtf8("pushButton_push2box"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_push2box->sizePolicy().hasHeightForWidth());
        pushButton_push2box->setSizePolicy(sizePolicy1);
        pushButton_push2box->setMinimumSize(QSize(20, 41));
        pushButton_push2box->setMaximumSize(QSize(30, 41));

        horizontalLayout->addWidget(pushButton_push2box);

        pushButton_paste = new QPushButton(horizontalGroupBox);
        pushButton_paste->setObjectName(QString::fromUtf8("pushButton_paste"));
        pushButton_paste->setMinimumSize(QSize(60, 41));
        pushButton_paste->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_paste);

        pushButton_clear = new QPushButton(horizontalGroupBox);
        pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
        pushButton_clear->setMinimumSize(QSize(60, 41));
        pushButton_clear->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_clear);

        pushButton_collect = new QPushButton(horizontalGroupBox);
        pushButton_collect->setObjectName(QString::fromUtf8("pushButton_collect"));
        sizePolicy.setHeightForWidth(pushButton_collect->sizePolicy().hasHeightForWidth());
        pushButton_collect->setSizePolicy(sizePolicy);
        pushButton_collect->setMinimumSize(QSize(60, 41));
        pushButton_collect->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_collect);

        pushButton_baidu = new QPushButton(horizontalGroupBox);
        pushButton_baidu->setObjectName(QString::fromUtf8("pushButton_baidu"));
        sizePolicy.setHeightForWidth(pushButton_baidu->sizePolicy().hasHeightForWidth());
        pushButton_baidu->setSizePolicy(sizePolicy);
        pushButton_baidu->setMinimumSize(QSize(60, 41));
        pushButton_baidu->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_baidu);

        pushButton_biying = new QPushButton(horizontalGroupBox);
        pushButton_biying->setObjectName(QString::fromUtf8("pushButton_biying"));
        sizePolicy.setHeightForWidth(pushButton_biying->sizePolicy().hasHeightForWidth());
        pushButton_biying->setSizePolicy(sizePolicy);
        pushButton_biying->setMinimumSize(QSize(60, 41));
        pushButton_biying->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_biying);

        pushButton_360so = new QPushButton(horizontalGroupBox);
        pushButton_360so->setObjectName(QString::fromUtf8("pushButton_360so"));
        sizePolicy.setHeightForWidth(pushButton_360so->sizePolicy().hasHeightForWidth());
        pushButton_360so->setSizePolicy(sizePolicy);
        pushButton_360so->setMinimumSize(QSize(60, 41));
        pushButton_360so->setMaximumSize(QSize(106, 41));

        horizontalLayout->addWidget(pushButton_360so);

        checkBox_forcelocalhost = new QCheckBox(horizontalGroupBox);
        checkBox_forcelocalhost->setObjectName(QString::fromUtf8("checkBox_forcelocalhost"));

        horizontalLayout->addWidget(checkBox_forcelocalhost);

        checkBox_dict_realtime = new QCheckBox(horizontalGroupBox);
        checkBox_dict_realtime->setObjectName(QString::fromUtf8("checkBox_dict_realtime"));

        horizontalLayout->addWidget(checkBox_dict_realtime);

        checkBox_fileautoload = new QCheckBox(horizontalGroupBox);
        checkBox_fileautoload->setObjectName(QString::fromUtf8("checkBox_fileautoload"));

        horizontalLayout->addWidget(checkBox_fileautoload);

        checkBox_watchhttp = new QCheckBox(horizontalGroupBox);
        checkBox_watchhttp->setObjectName(QString::fromUtf8("checkBox_watchhttp"));

        horizontalLayout->addWidget(checkBox_watchhttp);

        checkBox_tree = new QCheckBox(horizontalGroupBox);
        checkBox_tree->setObjectName(QString::fromUtf8("checkBox_tree"));

        horizontalLayout->addWidget(checkBox_tree);

        checkBox_openfirstitem = new QCheckBox(horizontalGroupBox);
        checkBox_openfirstitem->setObjectName(QString::fromUtf8("checkBox_openfirstitem"));

        horizontalLayout->addWidget(checkBox_openfirstitem);

        lineEdit_sendnum = new QLineEdit(horizontalGroupBox);
        lineEdit_sendnum->setObjectName(QString::fromUtf8("lineEdit_sendnum"));
        sizePolicy.setHeightForWidth(lineEdit_sendnum->sizePolicy().hasHeightForWidth());
        lineEdit_sendnum->setSizePolicy(sizePolicy);
        lineEdit_sendnum->setMinimumSize(QSize(50, 30));
        lineEdit_sendnum->setMaximumSize(QSize(25, 41));
        lineEdit_sendnum->setMaxLength(300);

        horizontalLayout->addWidget(lineEdit_sendnum);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(horizontalGroupBox);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(131, 16));
        label->setMaximumSize(QSize(16777215, 30));
        label->setFont(font);

        verticalLayout->addWidget(label);

        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        textEdit = new QTextEdit(splitter);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setMaximumSize(QSize(16777215, 16777215));
        textEdit->setFont(font);
        textEdit->setStyleSheet(QString::fromUtf8(""));
        splitter->addWidget(textEdit);
        textEdit_cmdresult = new QTextEdit(splitter);
        textEdit_cmdresult->setObjectName(QString::fromUtf8("textEdit_cmdresult"));
        textEdit_cmdresult->setMaximumSize(QSize(16777215, 16777215));
        textEdit_cmdresult->setFont(font);
        textEdit_cmdresult->setStyleSheet(QString::fromUtf8(""));
        splitter->addWidget(textEdit_cmdresult);

        verticalLayout->addWidget(splitter);


        verticalLayout_3->addLayout(verticalLayout);


        gridLayout->addLayout(verticalLayout_3, 0, 1, 3, 1);

        splitter_4 = new QSplitter(centralWidget);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Horizontal);
        comboBox_findlist = new QComboBox(splitter_4);
        comboBox_findlist->setObjectName(QString::fromUtf8("comboBox_findlist"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBox_findlist->sizePolicy().hasHeightForWidth());
        comboBox_findlist->setSizePolicy(sizePolicy2);
        comboBox_findlist->setMaximumSize(QSize(16777215, 30));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Times New Roman"));
        font2.setPointSize(14);
        comboBox_findlist->setFont(font2);
        comboBox_findlist->setEditable(true);
        comboBox_findlist->setMaxVisibleItems(30);
        splitter_4->addWidget(comboBox_findlist);
        pushButton = new QPushButton(splitter_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(20, 0));
        pushButton->setMaximumSize(QSize(120, 16777215));
        splitter_4->addWidget(pushButton);

        gridLayout->addWidget(splitter_4, 1, 0, 1, 1);

        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        treeView_ut = new QTreeView(splitter_2);
        treeView_ut->setObjectName(QString::fromUtf8("treeView_ut"));
        splitter_2->addWidget(treeView_ut);
        listWidget_cmdlist = new QListWidget(splitter_2);
        listWidget_cmdlist->setObjectName(QString::fromUtf8("listWidget_cmdlist"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font3.setPointSize(12);
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(50);
        listWidget_cmdlist->setFont(font3);
        listWidget_cmdlist->setStyleSheet(QString::fromUtf8("selection-color: rgb(0, 255, 255);"));
        splitter_2->addWidget(listWidget_cmdlist);

        gridLayout->addWidget(splitter_2, 2, 0, 1, 1);

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
        label_3->setText(QApplication::translate("MainWindow", "\345\221\275\344\273\244\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        checkBox_autosend->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
        pushButton_delkey->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\345\205\263\351\224\256\345\255\227", 0, QApplication::UnicodeUTF8));
        checkBox_treeopen->setText(QApplication::translate("MainWindow", "\345\217\214\345\207\273\346\211\223\345\274\200\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        pushButton_connect->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_push2box->setToolTip(QApplication::translate("MainWindow", "\346\267\273\345\212\240\345\210\260\346\211\247\350\241\214\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_push2box->setText(QApplication::translate("MainWindow", "+", 0, QApplication::UnicodeUTF8));
        pushButton_paste->setText(QApplication::translate("MainWindow", "\347\262\230\350\264\264", 0, QApplication::UnicodeUTF8));
        pushButton_clear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
        pushButton_collect->setText(QApplication::translate("MainWindow", "\346\224\266\350\227\217", 0, QApplication::UnicodeUTF8));
        pushButton_baidu->setText(QApplication::translate("MainWindow", "\347\231\276\345\272\246", 0, QApplication::UnicodeUTF8));
        pushButton_biying->setText(QApplication::translate("MainWindow", "\345\277\205\345\272\224", 0, QApplication::UnicodeUTF8));
        pushButton_360so->setText(QApplication::translate("MainWindow", "360so", 0, QApplication::UnicodeUTF8));
        checkBox_forcelocalhost->setText(QApplication::translate("MainWindow", "Local", 0, QApplication::UnicodeUTF8));
        checkBox_dict_realtime->setText(QApplication::translate("MainWindow", "\345\255\227\345\205\270\345\256\236\346\227\266", 0, QApplication::UnicodeUTF8));
        checkBox_fileautoload->setText(QApplication::translate("MainWindow", " \346\226\207\344\273\266\345\256\236\346\227\266\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        checkBox_watchhttp->setText(QApplication::translate("MainWindow", "watch http", 0, QApplication::UnicodeUTF8));
        checkBox_tree->setText(QApplication::translate("MainWindow", "tree", 0, QApplication::UnicodeUTF8));
        checkBox_openfirstitem->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\351\246\226\351\241\271", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_sendnum->setToolTip(QApplication::translate("MainWindow", "\346\211\247\350\241\214\346\225\260\351\207\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\211\247\350\241\214\345\221\275\344\273\244", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
