TEMPLATE = app
TARGET   = qtestceplib-base
DESTDIR  = ../bin/

include(../../qceplib-base.pri)

HEADERS += \
    qtestceplibmainwindow.h

SOURCES += \
    qtestceplibmain.cpp \
    qtestceplibmainwindow.cpp

FORMS += \
    qtestceplibmainwindow.ui
