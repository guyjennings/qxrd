TEMPLATE = app
TARGET   = qtestceplib-hdf5
DESTDIR  = ../bin/

CONFIG += qceplib-base qceplib-szip qceplib-zlib qceplib-hdf5 qceplib-qwt

include(../../qceplib-base.pri)
include(../../qceplib-szip.pri)
include(../../qceplib-zlib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-qwt.pri)

HEADERS += \
    qtestceplibhdf5mainwindow.h \
    qtestceplibhdf5document.h

SOURCES += \
    qtestceplibhdf5main.cpp \
    qtestceplibhdf5mainwindow.cpp \
    qtestceplibhdf5document.cpp

FORMS += \
    qtestceplibhdf5mainwindow.ui
