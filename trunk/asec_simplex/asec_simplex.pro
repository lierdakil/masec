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
    INCLUDEPATH += C:\Qwt-5.1.2-svn\include
    LIBS += -L \
        C:\Qwt-5.1.2-svn\lib \
        -lqwtd5
    INCLUDEPATH += \
        #"C:\Program Files\GnuWin32\include" \
        "C:\msys\1.0\local\include"
    LIBS += \
        #-L "C:\Program Files\GnuWin32\lib" \
        -L "C:\msys\1.0\local\lib" \
        -lgsl \
        -lgslcblas
}
else {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5
    LIBS += -lqwt \
        -lgsl \
        -lgslcblas
    QMAKE_CXXFLAGS+= -fopenmp
    QMAKE_LFLAGS+= -fopenmp
}
FORMS += graph.ui
