TEMPLATE = app
TARGET   = qtestceplib-nexus
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-nexus.pri)

HEADERS += \
    qtestceplibnexusmainwindow.h

SOURCES += \
    qtestceplibnexusmain.cpp \
    qtestceplibnexusmainwindow.cpp

FORMS += \
    qtestceplibnexusmainwindow.ui
