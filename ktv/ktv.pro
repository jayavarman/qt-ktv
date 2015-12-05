#-------------------------------------------------
#
# Project created by QtCreator 2015-11-04T15:12:42
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ktv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    songlistitem.cpp \
    ktvtcpserver.cpp \
    ktvthread.cpp

HEADERS  += mainwindow.h \
    songlistitem.h \
    ktvtcpserver.h \
    ktvthread.h

FORMS    += mainwindow.ui
