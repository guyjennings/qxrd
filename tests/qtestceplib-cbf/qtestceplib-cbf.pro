TEMPLATE = app
TARGET   = qtestceplib-cbf
DESTDIR  = ../bin/

include(../../qceplib-base.pri)
include(../../qceplib-szip.pri)
include(../../qceplib-zlib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-cbf.pri)
#include(../../qceplib-tiff.pri)

HEADERS += \
    qtestceplibcbfmainwindow.h \
    qtestceplibcbfdocument.h

SOURCES += \
    qtestceplibcbfmain.cpp \
    qtestceplibcbfmainwindow.cpp \
    qtestceplibcbfdocument.cpp

FORMS += \
    qtestceplibcbfmainwindow.ui
