TEMPLATE = app
DESTDIR = ../build/
TARGET = asec_temperature
QT += core \
    gui
CONFIG += qdbus
HEADERS += adaptors.h \
    ctrl/temp.h \
    QTempTimeline.h \
    settings.h \
    asec_temperature.h
SOURCES += adaptors.cpp \
    QTempTimeline.cpp \
    settings.cpp \
    main.cpp \
    asec_temperature.cpp
FORMS += settings.ui \
    asec_temperature.ui
RESOURCES += 
INCLUDEPATH += ../asec_gpib/ \
    ../asec_reply/
LIBS += ../lib/sicl32.lib
