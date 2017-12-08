#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T21:24:51
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MazeGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    field.cpp

HEADERS  += mainwindow.h \
    cell.h \
    field.h \
    node.h

FORMS    += mainwindow.ui
