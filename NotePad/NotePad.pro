#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T16:58:00
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += printsupport

CONFIG += c++11

TARGET = NotePad
TEMPLATE = app


SOURCES += main.cpp\
    FindDialog.cpp \
    MainWindowSlots.cpp \
    MainWindowUI.cpp

HEADERS  += MainWindow.h \
    FindDialog.h

RESOURCES += \
    Res.qrc
