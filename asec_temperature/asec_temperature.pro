TEMPLATE = app
DESTDIR = ../build/
TARGET = asec_temperature
QT += core \
    gui \
    dbus
CONFIG += exceptions
HEADERS += adaptors.h \
    ctrl/temp.h \
    QTempTimeline.h \
    settings.h \
    asec_temperature.h
SOURCES += ctrl/temp.cpp \
    adaptors.cpp \
    QTempTimeline.cpp \
    settings.cpp \
    main.cpp \
    asec_temperature.cpp
FORMS += settings.ui \
    asec_temperature.ui
RESOURCES +=
INCLUDEPATH += ../asec_gpib \
        ../visa
LIBS += -L../build \
    -lasec_gpib

win32 {
    INCLUDEPATH += C:\Qwt-5.2.1-svn\include
    LIBS += -L C:\Qwt-5.2.1-svn\lib
    LIBS += -lqwt
}
unix {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5 \
        /usr/include/qwt-qt4
    LIBS += -lqwt
}
