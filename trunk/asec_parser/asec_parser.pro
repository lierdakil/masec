TEMPLATE = app
DESTDIR = ../build
TARGET = asec_parser
CONFIG += console release
QT += core \
    gui
HEADERS += mainwindow.h
SOURCES += main.cpp \
    mainwindow.cpp
FORMS += mainwindow.ui
RESOURCES +=
