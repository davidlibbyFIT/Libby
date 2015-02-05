#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T16:31:24
#
#-------------------------------------------------

QT       += core gui network

win32:DEFINES       += WIN32
android:DEFINES     += ANDROID
unix:DEFINES        += UNIX

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlowMaster
TEMPLATE = app


SOURCES += main.cpp\
        mainview.cpp

HEADERS  += mainview.h

FORMS    += mainview.ui

CONFIG += mobility
MOBILITY = 

