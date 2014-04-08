TEMPLATE = app
TARGET   = qtestceplib-hdf5
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-szip.pri)
include(../../qceplib-zlib.pri)
include(../../qceplib-hdf5.pri)

HEADERS += \
    qtestceplibhdf5mainwindow.h

SOURCES += \
    qtestceplibhdf5main.cpp \
    qtestceplibhdf5mainwindow.cpp

FORMS += \
    qtestceplibhdf5mainwindow.ui
