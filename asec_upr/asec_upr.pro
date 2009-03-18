TEMPLATE = app
DESTDIR = ../build/
TARGET = asec_upr
HEADERS += adaptors.h \
    measurethread.h \
    ctrl/vol.h \
    ctrl/osc.h \
    ctrl/gen.h \
    asec_upr.h
SOURCES += ctrl/vol.cpp \
    ctrl/gen.cpp \
    ctrl/osc.cpp \
    adaptors.cpp \
    measurethread.cpp \
    main.cpp \
    asec_upr.cpp
FORMS += asec_upr.ui
INCLUDEPATH += ../asec_gpib/
LIBS += ../lib/visa32.lib
QT += core \
    gui \
    dbus
