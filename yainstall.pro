#-------------------------------------------------
#
# Project created by QtCreator 2013-01-24T23:53:37
#
#-------------------------------------------------

QT       += core gui network
lessThan(QT_MAJOR_VERSION, 5): QT += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

DEFINES += MAX_VERSION="1.2"
DEFINES += MIN_VERSION="0"

TARGET = yainstall
TEMPLATE = app

unix:!macx {
    LIBS += -lzip -lz
    target.path = /usr/bin
}
macx {
    INCLUDEPATH += /opt/local/include /opt/local/lib/libzip/include
    LIBS += -L/opt/local/lib -lzip -lz
}

# for development environment
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../yalib/release/ -lyalib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../yalib/debug/ -lyalib
else:mac: LIBS += -F$$PWD/../yalib/ -framework yalib
else:unix: LIBS += -L$$PWD/../yalib/ -lyalib

INCLUDEPATH += $$PWD/../yalib
DEPENDPATH += $$PWD/../yalib

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

INSTALLS += target
