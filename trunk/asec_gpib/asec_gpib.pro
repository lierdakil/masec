# -------------------------------------------------
# Project created by QtCreator 2009-03-20T15:23:35
# -------------------------------------------------
QT -= gui
TARGET = asec_gpib
TEMPLATE = lib
DEFINES += ASEC_GPIB_LIBRARY
SOURCES += gpibctrl.cpp \
    gpibgenericexception.cpp \
    gpibvisaexception.cpp \
    gpibidnexception.cpp
HEADERS += gpibctrl.h \
    asec_gpib_global.h \
    gpibgenericexception.h \
    gpibvisaexception.h \
    gpibidnexception.h \
    gpibexceptions.h \
    visa.h
LIBS = -L../visa \
    -lvisa
