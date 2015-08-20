#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T14:47:42
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab1Desktop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tableview.cpp \
    label.cpp \
    customview.cpp \
    customwidget.cpp

HEADERS  += mainwindow.h \
    tableview.h \
    label.h \
    customview.h \
    customwidget.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    resource.qrc
