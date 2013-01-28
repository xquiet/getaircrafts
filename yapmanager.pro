#-------------------------------------------------
#
# Project created by QtCreator 2013-01-24T23:53:37
#
#-------------------------------------------------

QT       += core gui webkit network

DEFINES += MAX_VERSION="1.0"
DEFINES += MIN_VERSION="0"

TARGET = yapmanager
TEMPLATE = app

LIBS += -lzip -lz

SOURCES += main.cpp\
        mainwindow.cpp \
    webservice.cpp \
    Installer.cpp

HEADERS  += mainwindow.h \
    webservice.h \
    Installer.h

FORMS    += mainwindow.ui

RESOURCES += \
    yapmanager.qrc
