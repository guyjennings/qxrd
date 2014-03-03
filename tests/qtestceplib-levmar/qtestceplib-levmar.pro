TEMPLATE = app
TARGET   = qtestceplib-levmar
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-levmar.pri)

HEADERS += \
    qtestcepliblevmarmainwindow.h

SOURCES += \
    qtestcepliblevmarmain.cpp \
    qtestcepliblevmarmainwindow.cpp

FORMS += \
    qtestcepliblevmarmainwindow.ui
