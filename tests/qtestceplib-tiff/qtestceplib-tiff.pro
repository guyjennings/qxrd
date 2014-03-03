TEMPLATE = app
TARGET   = qtestceplib-tiff
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-tiff.pri)

HEADERS += \
    qtestceplibtiffmainwindow.h

SOURCES += \
    qtestceplibtiffmain.cpp \
    qtestceplibtiffmainwindow.cpp

FORMS += \
    qtestceplibtiffmainwindow.ui
