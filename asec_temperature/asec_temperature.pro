TEMPLATE = app
TARGET = ../../build/asec_temperature
QT += core \
    gui
CONFIG += qdbus
HEADERS += ctrl/temp.h \
    QTempTimeline.h \
    settings.h \
    asec_temperature.h
SOURCES += QTempTimeline.cpp \
    settings.cpp \
    main.cpp \
    asec_temperature.cpp
FORMS += settings.ui \
    asec_temperature.ui
RESOURCES +=
INCLUDEPATH += ../asec_gpib/
LIBS += ../lib/sicl32.lib