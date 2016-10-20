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
    dataanalysis.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    preprocess.h \
    dataanalysis.h \
    about.h

FORMS    += mainwindow.ui \
    preprocess.ui \
    dataanalysis.ui \
    about.ui

RESOURCES += \
	main.qrc

RC_FILE += \
	main.rc
