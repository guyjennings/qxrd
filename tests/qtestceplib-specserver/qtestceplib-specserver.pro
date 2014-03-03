TEMPLATE = app
TARGET   = qtestceplib-specserver
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-specserver.pri)

HEADERS += \
    qtestceplibspecservermainwindow.h

SOURCES += \
    qtestceplibspecservermain.cpp \
    qtestceplibspecservermainwindow.cpp

FORMS += \
    qtestceplibspecservermainwindow.ui
