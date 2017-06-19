TEMPLATE = app
TARGET   = qtestceplib-tiff
DESTDIR  = ../bin/

CONFIG += qceplib-tiff

include(../../qceplib-base.pri)
include(../../qceplib-tiff.pri)

HEADERS += \
    qtestceplibtiffmainwindow.h \
    qtestceplibtiffdocument.h

SOURCES += \
    qtestceplibtiffmain.cpp \
    qtestceplibtiffmainwindow.cpp \
    qtestceplibtiffdocument.cpp

FORMS += \
    qtestceplibtiffmainwindow.ui
