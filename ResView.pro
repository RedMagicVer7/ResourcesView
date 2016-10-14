#-------------------------------------------------
#
# Project created by QtCreator 2016-10-12T18:31:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ResView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    resourcestable.cpp \
    pe.cpp \
    TextItem.cpp

HEADERS  += mainwindow.h \
    resourcestable.h \
    pe.h \
    TextItem.h

FORMS    += mainwindow.ui
