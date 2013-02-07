#-------------------------------------------------
#
# Project created by QtCreator 2013-01-24T23:53:37
#
#-------------------------------------------------

QT       += core gui webkit network

DEFINES += MAX_VERSION="1.0"
DEFINES += MIN_VERSION="0"

TARGET = yainstall
TEMPLATE = app

unix:!macx {
    LIBS += -lzip -lz
}
macx {
    INCLUDEPATH += /opt/local/include /opt/local/lib/libzip/include
    LIBS += -L/opt/local/lib -lzip -lz
}

SOURCES += main.cpp\
        mainwindow.cpp \
    webservice.cpp \
    Installer.cpp \
    xml2html.cpp

HEADERS  += mainwindow.h \
    webservice.h \
    Installer.h \
    xml2html.h

FORMS    += mainwindow.ui

RESOURCES += \
    yainstall.qrc
