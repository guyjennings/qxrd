TEMPLATE = app
TARGET   = qtestceplib-combined
DESTDIR  = ../bin/

CONFIG += qceplib-base qceplib-szip qceplib-zlib qceplib-hdf5 qceplib-cbf qceplib-mar345 qceplib-nexus qceplib-specserver
CONFIG += qceplib-tiff qceplib-levmar qceplib-qwt

include(../../qceplib-base.pri)
include(../../qceplib-szip.pri)
include(../../qceplib-zlib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-cbf.pri)
include(../../qceplib-mar345.pri)
include(../../qceplib-nexus.pri)
include(../../qceplib-specserver.pri)
include(../../qceplib-tiff.pri)
include(../../qceplib-levmar.pri)
include(../../qceplib-qwt.pri)

HEADERS += \
    qtestceplibcombinedmainwindow.h \
    qtestceplibcombineddocument.h

SOURCES += \
    qtestceplibcombinedmain.cpp \
    qtestceplibcombinedmainwindow.cpp \
    qtestceplibcombineddocument.cpp

FORMS += \
    qtestceplibcombinedmainwindow.ui
