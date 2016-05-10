TEMPLATE = app
TARGET   = qtestceplib-mar345
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-mar345.pri)

HEADERS += \
    qtestceplibmar345mainwindow.h \
    qtestceplibmar345document.h

SOURCES += \
    qtestceplibmar345main.cpp \
    qtestceplibmar345mainwindow.cpp \
    qtestceplibmar345document.cpp

FORMS += \
    qtestceplibmar345mainwindow.ui
