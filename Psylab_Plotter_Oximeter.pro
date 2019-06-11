#-------------------------------------------------
#
# Project created by QtCreator 2016-01-06T17:24:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network

TARGET = BloodOxy_draw
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    libpsyclass.cpp \
    darw.cpp \
    qcustomplot.cpp \
    about.cpp \
    histowindow.cpp \
    odi.cpp

HEADERS  += mainwindow.h \
    libpsyclass.h \
    darw.hpp \
    qcustomplot.h \
    about.h \
    histowindow.h \
    odi.hpp

FORMS    += mainwindow.ui \
    about.ui \
    histowindow.ui

TRANSLATIONS += zh_TW.ts \
    en_US.ts
