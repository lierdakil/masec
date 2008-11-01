TEMPLATE = app
TARGET = ../../build/asec_control
QT += core \
    gui \
    script \
    dbus
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
