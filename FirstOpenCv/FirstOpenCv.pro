#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T19:02:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FirstOpenCv
TEMPLATE = app

LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
