#-------------------------------------------------
#
# Project created by QtCreator 2013-01-24T23:53:37
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -fPIC -std=gnu++0x

QT       += core gui network
lessThan(QT_MAJOR_VERSION, 5): QT += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

MAX_VERSION = 1
MIN_VERSION = 3
PATCH_VERSION = 0

DEFINES += MAX_VERSION=$$MAX_VERSION
DEFINES += MIN_VERSION=$$MIN_VERSION
DEFINES += PATCH_VERSION=$$PATCH_VERSION
DEFINES += STRVERSION=\\\"$${MAX_VERSION}.$${MIN_VERSION}.$${PATCH_VERSION}\\\"

TARGET = getaircrafts
TEMPLATE = app

VERSION = $${MAX_VERSION}.$${MIN_VERSION}.$${PATCH_VERSION}

unix:!macx {
    LIBS += -lzip -lz
    target.path = /usr/bin
}
macx {
    INCLUDEPATH += /opt/local/include /opt/local/lib/libzip/include
    LIBS += -L/opt/local/lib -lzip -lz
}
win32 {
    CONFIG += windows
    INCLUDEPATH += C:\Users\Matteo\workspace\libzip-win-build\lib
    INCLUDEPATH += C:\Users\Matteo\workspace\libzip-win-build\win32
    LIBS += -LC:\Users\Matteo\workspace\libzip-win-build\build-VS2019\Release -lz -lzip
}

# for development environment
# libyalib for both release and debug as I'm building libyalib as RelWithDebInfo

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../yalib-build/src/ -llibyalib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../yalib-build/src/ -llibyalib
else:mac: LIBS += -F$$PWD/../yalib-build/src/ -framework yalib
else:unix: LIBS += -L$$PWD/../yalib-build/src/ -llibyalib

INCLUDEPATH += $$PWD/../yalib/src
DEPENDPATH += $$PWD/../yalib/src

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
    getaircrafts.qrc

INSTALLS += target
