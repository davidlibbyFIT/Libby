#-------------------------------------------------
#
# Project created by QtCreator 2014-12-31T09:50:33
#
#-------------------------------------------------

QT       += core gui network

win32:DEFINES       += WIN32
android:DEFINES     += ANDROID
unix:DEFINES        += UNIX


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EchoServer
TEMPLATE = app


SOURCES += main.cpp\
        mainview.cpp

HEADERS  += mainview.h

FORMS    += mainview.ui

QMAKE_CXXFLAGS += -std=c++11

CONFIG += mobility
MOBILITY = 

