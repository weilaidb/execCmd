#-------------------------------------------------
#
# Project created by QtCreator 2018-02-10T21:25:07
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = execWinCmdServer2
TEMPLATE = app
RC_FILE = app.rc


SOURCES += main.cpp\
    sockthread.cpp \
    msgtipsautoshut.cpp \
    msgtips.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h \
    sockthread.h \
    msgtipsautoshut.h \
    msgtips.h

FORMS    += mainwindow.ui

RESOURCES += \
    imgs.qrc
