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
unix {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5 \
        /usr/include/qwt-qt4
}

system($$QMAKE_CXX dummy.cpp -o $$DESTDIR/dummy){
    message(Enabling OpenMP support...)
    QMAKE_CXXFLAGS+= -fopenmp -DOPENMP
    QMAKE_LFLAGS+= -fopenmp
}

#Debian uses -lqwt-qt4
LIBS += -lqwt \
    -lgsl \
    -lgslcblas
FORMS += graph.ui
