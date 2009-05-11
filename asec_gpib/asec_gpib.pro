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
    gpibidnexception.cpp \
    gpibinvalidreplyexception.cpp \
    gpibunxparamvalexception.cpp
HEADERS += gpibctrl.h \
    asec_gpib_global.h \
    gpibgenericexception.h \
    gpibidnexception.h \
    gpibexceptions.h \
    gpibinvalidreplyexception.h \
    gpibunxparamvalexception.h
win32{
	LIBS = -L../visa \
	    -lvisa32
	INCLUDEPATH += ../visa
	DEFINES += VISA
	SOURCES += gpibvisaexception.cpp
	HEADERS += gpibvisaexception.h
}
unix{
	LIBS = -lgpib
	DEFINES+=LINUXGPIB
	SOURCES += gpibbusexception.cpp
	HEADERS += gpibbusexception.h
}
