TEMPLATE = app
TARGET   = qtestceplib-combined
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-cbf.pri)
include(../../qceplib-mar345.pri)
include(../../qceplib-nexus.pri)
include(../../qceplib-qwt.pri)
include(../../qceplib-specserver.pri)
include(../../qceplib-tiff.pri)
include(../../qceplib-levmar.pri)

HEADERS += \
    qtestceplibcombinedmainwindow.h

SOURCES += \
    qtestceplibcombinedmain.cpp \
    qtestceplibcombinedmainwindow.cpp

FORMS += \
    qtestceplibcombinedmainwindow.ui
