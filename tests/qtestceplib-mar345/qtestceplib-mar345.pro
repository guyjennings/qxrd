TEMPLATE = app
TARGET   = qtestceplib-mar345
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-mar345.pri)

HEADERS += \
    qtestceplibmar345mainwindow.h

SOURCES += \
    qtestceplibmar345main.cpp \
    qtestceplibmar345mainwindow.cpp

FORMS += \
    qtestceplibmar345mainwindow.ui
