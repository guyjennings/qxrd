TEMPLATE = app
TARGET   = qtestceplib-levmar
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-levmar.pri)

HEADERS += \
    qtestcepliblevmarmainwindow.h \
    qtestcepliblevmardocument.h

SOURCES += \
    qtestcepliblevmarmain.cpp \
    qtestcepliblevmarmainwindow.cpp \
    qtestcepliblevmardocument.cpp

FORMS += \
    qtestcepliblevmarmainwindow.ui
