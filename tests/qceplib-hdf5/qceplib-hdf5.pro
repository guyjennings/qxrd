TEMPLATE = app
TARGET   = qtestceplib-hdf5
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-hdf5.pri)

HEADERS += \
    qtestceplibhdf5mainwindow.h

SOURCES += \
    qtestceplibhdf5main.cpp \
    qtestceplibhdf5mainwindow.cpp

FORMS += \
    qtestceplibhdf5mainwindow.ui
