TEMPLATE = app
TARGET   = qtestceplib-tiff
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-tiff.pri)

HEADERS += \
    qtestceplibtiffmainwindow.h

SOURCES += \
    qtestceplibtiffmain.cpp \
    qtestceplibtiffmainwindow.cpp

FORMS += \
    qtestceplibtiffmainwindow.ui
