#-------------------------------------------------
#
# Project created by QtCreator 2016-09-27T20:45:36
#
#-------------------------------------------------

QT       += core gui \
            serialport

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scanner3D
TEMPLATE = app

INCLUDEPATH +=  /usr/local/include/opencv
LIBS        +=  `pkg-config --libs opencv` \
                -lglut -lGLU

SOURCES +=  main.cpp\
            mainwindow.cpp \
            scan.cpp \
    worker.cpp \
    renderer.cpp \
    pointcloud.cpp \
    oglwidget.cpp

HEADERS  += mainwindow.h \
            scan.h \
    worker.h \
    renderer.h \
    pointcloud.h \
    oglwidget.h

FORMS    += mainwindow.ui \
    renderer.ui
