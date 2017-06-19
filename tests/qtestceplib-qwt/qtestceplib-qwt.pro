TEMPLATE = app
TARGET   = qtestceplib-qwt
DESTDIR  = ../bin/

include(../../qceplib-qwt.pri)
include(../../qceplib-base.pri)

HEADERS += \
    qtestceplibqwtmainwindow.h \
    qtestceplibqwtdocument.h

SOURCES += \
    qtestceplibqwtmain.cpp \
    qtestceplibqwtmainwindow.cpp \
    qtestceplibqwtdocument.cpp

FORMS += \
    qtestceplibqwtmainwindow.ui
