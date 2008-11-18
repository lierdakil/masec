TEMPLATE = app
DESTDIR = ../build/
TARGET = asec_upr
HEADERS += measurethread.h \
    ctrl/vol.h \
    ctrl/osc.h \
    ctrl/gen.h \
    measure.h \
    asec_upr.h
SOURCES += measurethread.cpp \
    measure.cpp \
    main.cpp \
    asec_upr.cpp
FORMS += asec_upr.ui
INCLUDEPATH += ../asec_gpib/ \
    ../asec_reply/
LIBS += ../lib/sicl32.lib
QT += core \
    gui \
    dbus
