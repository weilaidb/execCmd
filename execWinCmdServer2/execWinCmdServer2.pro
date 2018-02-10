#-------------------------------------------------
#
# Project created by QtCreator 2018-02-10T21:25:07
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = execWinCmdServer2
TEMPLATE = app
RESOURCES += \
    imgs.qrc
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

OTHER_FILES += \
    Makefile.Release \
    Makefile.Debug \
    Makefile \
    execWinCmdServer2.pro.user \
    execWinCmdServer2.pro \
    app.rc \
    app.ico

RESOURCES += \
    imgs.qrc
