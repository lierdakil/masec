TEMPLATE = app
TARGET = ../../build/asec_upr
HEADERS += ctrl/vol.h \
    ctrl/osc.h \
    ctrl/gpib.h \
    ctrl/gen.h \
    ctrl/sicl.h \
    sleep.h \
    sleep.h \
    measure.h \
    asec_upr.h
SOURCES += measure.cpp \
    main.cpp \
    asec_upr.cpp
FORMS += asec_upr.ui
INCLUDEPATH += ../asec_gpib/
LIBS += ../lib/sicl32.lib
QT += core \
    gui \
    dbus
