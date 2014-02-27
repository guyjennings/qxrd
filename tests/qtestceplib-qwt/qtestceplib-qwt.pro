TEMPLATE = app
TARGET   = qtestceplib-qwt
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-qwt.pri)

HEADERS += \
    qtestceplibqwtmainwindow.h

SOURCES += \
    qtestceplibqwtmain.cpp \
    qtestceplibqwtmainwindow.cpp

FORMS += \
    qtestceplibqwtmainwindow.ui
