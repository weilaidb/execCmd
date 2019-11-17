#-------------------------------------------------
#
# Project created by QtCreator 2018-02-05T19:51:45
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = cmdServer
TEMPLATE = app

RC_FILE = app.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    sockthread.cpp \
    msgtips.cpp \
    msgtipsautoshut.cpp \
    cgetfilecheckxxsum.cpp \
    processbarx.cpp

HEADERS  += mainwindow.h \
    sockthread.h \
    msgtips.h \
    msgtipsautoshut.h \
    version.h \
    cgetfilecheckxxsum.h \
    processbarx.h

FORMS    += mainwindow.ui
