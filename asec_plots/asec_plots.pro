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
INCLUDEPATH += C:\Qwt-2-svn\mingw\include
LIBS += C:\Qwt-2-svn\mingw\lib\libqwtd5.a
