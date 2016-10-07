#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T14:20:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PwdBox
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    preprocess.cpp \
    dataanalysis.cpp

HEADERS  += mainwindow.h \
    preprocess.h \
    dataanalysis.h

FORMS    += mainwindow.ui \
    preprocess.ui \
    dataanalysis.ui
