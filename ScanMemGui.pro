#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T21:35:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScanMemGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pidvarionts.cpp \
    mytablewidget.cpp \
    searchform.cpp \
    addvalue.cpp

HEADERS  += mainwindow.h \
    pidvarionts.h \
    mytablewidget.h \
    searchform.h \
    addvalue.h

FORMS    += mainwindow.ui \
    pidvarionts.ui \
    searchform.ui \
    addvalue.ui

QMAKE_CXXFLAGS += -std=c++0x
