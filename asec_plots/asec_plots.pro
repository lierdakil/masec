TEMPLATE = app
DESTDIR = ../build
TARGET = asec_plots
QT += core \
    gui \
    dbus
HEADERS += QListWidgetItemPlot.h \
    qplotparam.h \
    qplotwindow.h \
    asec_plots.h
SOURCES += QListWidgetItemPlot.cpp \
    qplotparam.cpp \
    qplotwindow.cpp \
    main.cpp \
    asec_plots.cpp
FORMS += qplotparam.ui \
    qplotwindow.ui \
    asec_plots.ui
RESOURCES +=
win32{
    INCLUDEPATH += C:\MinGW\include
} else {
    INCLUDEPATH += /usr/include \
        /usr/include/qwt5 \
        /usr/include/qwt-qt4
}
#Debian uses -lqwt-qt4 instead
LIBS += -lqwt
