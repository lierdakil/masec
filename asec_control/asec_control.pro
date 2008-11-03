TEMPLATE = app
DESTDIR = ../build
TARGET = asec_control
QT += core \
    gui \
    script \
    dbus \
    xml
HEADERS += scriptthread.h \
    cgraphiccontrol.h \
    ccontrolbus.h \
    numberedtextview.h \
    asec_control.h
SOURCES += scripthread.cpp \
    ccontrolbus.cpp \
    numberedtextview.cpp \
    main.cpp \
    asec_control.cpp
FORMS += asec_control.ui
RESOURCES += 
