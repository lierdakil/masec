TEMPLATE = app
DESTDIR = ../build/
TARGET = asec_magn
QT += core \
    gui
CONFIG += qdbus
HEADERS += QMagnTimeline.h \
    ctrl/magn.h \
    asec_magn.h \
    adaptors.h
SOURCES += QMagnTimeline.cpp \
    ctrl/magn.cpp \
    main.cpp \
    asec_magn.cpp \
    adaptors.cpp
FORMS += asec_magn.ui
RESOURCES += 
INCLUDEPATH += ../asec_gpib/
LIBS += ../lib/sicl32.lib
