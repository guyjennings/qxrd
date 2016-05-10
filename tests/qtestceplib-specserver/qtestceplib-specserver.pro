TEMPLATE = app
TARGET   = qtestceplib-specserver
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-specserver.pri)

HEADERS += \
    qtestceplibspecservermainwindow.h \
    qtestceplibspecserverdocument.h

SOURCES += \
    qtestceplibspecservermain.cpp \
    qtestceplibspecservermainwindow.cpp \
    qtestceplibspecserverdocument.cpp

FORMS += \
    qtestceplibspecservermainwindow.ui
