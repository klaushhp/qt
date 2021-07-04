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
    AboutDialog.cpp \
    AppConfig.cpp \
    FindDialog.cpp \
    MainWindowSlots.cpp \
    MainWindowUI.cpp \
    ReplaceDialog.cpp

HEADERS  += MainWindow.h \
    AboutDialog.h \
    AppConfig.h \
    FindDialog.h \
    ReplaceDialog.h

RESOURCES += \
    Res.qrc
