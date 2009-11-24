#-------------------------------------------------
#
# Project created by QtCreator 2009-09-17T15:18:48
#
#-------------------------------------------------

QT       -= core gui

TARGET = asec_xray
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = ../build/

TEMPLATE = app


SOURCES += main.cpp

win32 {
    INCLUDEPATH += C:\MinGW\include
}

unix {
    INCLUDEPATH += /usr/include
}

#Debian uses -lqwt-qt4
LIBS += -lgsl \
    -lgslcblas

