# -------------------------------------------------
# Project created by QtCreator 2009-03-20T15:23:35
# -------------------------------------------------
CONFIG += exceptions dll
QT -= gui
TARGET = asec_gpib
TEMPLATE = lib
DESTDIR = ../build
DEFINES += ASEC_GPIB_LIBRARY
SOURCES += gpibctrl.cpp \
    gpibgenericexception.cpp \
    gpibvisaexception.cpp \
    gpibidnexception.cpp \
    gpibinvalidreplyexception.cpp \
    gpibunxparamvalexception.cpp
HEADERS += gpibctrl.h \
    asec_gpib_global.h \
    gpibgenericexception.h \
    gpibvisaexception.h \
    gpibidnexception.h \
    gpibexceptions.h \
    gpibinvalidreplyexception.h \
    gpibunxparamvalexception.h
LIBS = -L../visa \
    -lvisa32
INCLUDEPATH += ../visa
