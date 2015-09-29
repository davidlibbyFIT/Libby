#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T14:15:30
#
#-------------------------------------------------

QT       += core
QT       += network
#QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VSSimTcpServer
##TEMPLATE = app

##CONFIG   += console
##CONFIG   -= app_bundle

SOURCES += main.cpp\
        mainwindow.cpp \
    mytcpserver.cpp

HEADERS  += mainwindow.h \
    mytcpserver.h

FORMS    += mainwindow.ui
