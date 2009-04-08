TEMPLATE = app
DESTDIR = ../build
TARGET = asec_control
QT += core \
    gui \
    script \
    dbus \
    xml
HEADERS += qfuncinitbuilder.h \
    qhelpindexbuilder.h \
    qmethodfetcher.h \
    scriptthread.h \
    ccontrolbus.h \
    numberedtextview.h \
    asec_control.h
SOURCES += qfuncinitbulder.cpp \
    qhelpindexbuilder.cpp \
    qmethodfetcher.cpp \
    scriptthread.cpp \
    ccontrolbus.cpp \
    numberedtextview.cpp \
    main.cpp \
    asec_control.cpp
FORMS += asec_control.ui
RESOURCES += 
