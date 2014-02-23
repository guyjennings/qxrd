TEMPLATE = app
TARGET   = qtestceplib-cbf
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-hdf5.pri)
include(../../qceplib-cbf.pri)

HEADERS += \
    qtestceplibcbfmainwindow.h

SOURCES += \
    qtestceplibcbfmain.cpp \
    qtestceplibcbfmainwindow.cpp

FORMS += \
    qtestceplibcbfmainwindow.ui
