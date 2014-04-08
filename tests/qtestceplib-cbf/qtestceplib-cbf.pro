TEMPLATE = app
TARGET   = qtestceplib-cbf
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-szip.pri)
include(../../qceplib-zlib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-cbf.pri)

HEADERS += \
    qtestceplibcbfmainwindow.h

SOURCES += \
    qtestceplibcbfmain.cpp \
    qtestceplibcbfmainwindow.cpp

FORMS += \
    qtestceplibcbfmainwindow.ui
