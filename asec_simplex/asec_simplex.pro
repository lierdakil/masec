# -------------------------------------------------
# Project created by QtCreator 2009-04-05T19:04:42
# -------------------------------------------------
QT += gui \
    core
TARGET = asec_simplex
TEMPLATE = app
CONFIG += console
DESTDIR = ../build/
SOURCES += point2d.cpp \
    main.cpp \
    graph.cpp \
    functions.cpp
HEADERS += point2d.h \
    graph.h \
    functions.h
win32 {
    INCLUDEPATH += C:\Qwt-5.2.0-svn\include \
        C:\gsl\include
    LIBS += -L C:\Qwt-5.2.0-svn\lib \
        -L "C:\gsl\lib"
}
else {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5
    QMAKE_CXXFLAGS+= -fopenmp
    QMAKE_LFLAGS+= -fopenmp
}
LIBS += -lqwt \
    -lgsl \
    -lgslcblas
FORMS += graph.ui
