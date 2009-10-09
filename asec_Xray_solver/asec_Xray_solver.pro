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
    INCLUDEPATH += C:\gsl\include
    LIBS += -L "C:\gsl\lib"
}

unix {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5 \
        /usr/include/qwt-qt4
    QMAKE_CXXFLAGS+= -fopenmp
    QMAKE_LFLAGS+= -fopenmp
}

#Debian uses -lqwt-qt4
LIBS += -lgsl \
    -lgslcblas

