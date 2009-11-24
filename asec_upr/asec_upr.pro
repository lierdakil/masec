CONFIG += exceptions
TEMPLATE = app
DESTDIR = ../build/
TARGET = asec_upr
HEADERS += adaptors.h \
    measurethread.h \
    ctrl/vol.h \
    ctrl/osc.h \
    ctrl/gen.h \
    asec_upr.h \
    measure_time.h
SOURCES += ctrl/vol.cpp \
    ctrl/gen.cpp \
    ctrl/osc.cpp \
    adaptors.cpp \
    measurethread.cpp \
    main.cpp \
    asec_upr.cpp
FORMS += asec_upr.ui
INCLUDEPATH += ../asec_gpib/ \
    ../visa/
LIBS += -L../build/ \
    -lasec_gpib
QT += core \
    gui \
    dbus

win32 {
    INCLUDEPATH += C:\Qwt-5.2.1-svn\include
    LIBS += -L C:\Qwt-5.2.1-svn\lib
    LIBS += -lqwt5
}
unix {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5 \
        /usr/include/qwt-qt4
    LIBS += -lqwt
}
