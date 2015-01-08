#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T17:51:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrackLaser1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

unix {
message("Using unix configuration")

LIBS += `pkg-config opencv --libs`

#OPENCV_PATH = /usr/local/opencv2/

#LIBS     += -L$$LIBS_PATH \
#            -lopencv_core \
#            -lopencv_highgui \
#            -lopencv_objdetect
}

INCLUDEPATH +=  /usr/local/include/
