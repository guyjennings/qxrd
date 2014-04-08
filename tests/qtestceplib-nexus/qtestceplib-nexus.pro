TEMPLATE = app
TARGET   = qtestceplib-nexus
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-szip.pri)
include(../../qceplib-zlib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-nexus.pri)

HEADERS += \
    qtestceplibnexusmainwindow.h

SOURCES += \
    qtestceplibnexusmain.cpp \
    qtestceplibnexusmainwindow.cpp

FORMS += \
    qtestceplibnexusmainwindow.ui
