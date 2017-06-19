TEMPLATE = app
TARGET   = qtestceplib-fileio
DESTDIR  = ../bin/

CONFIG += qceplib-tiff

include(../../qceplib-base.pri)
include(../../qceplib-tiff.pri)

HEADERS += \
    qtestceplibfileiomainwindow.h \
    qtestceplibfileiodocument.h

SOURCES += \
    qtestceplibfileiomain.cpp \
    qtestceplibfileiomainwindow.cpp \
    qtestceplibfileiodocument.cpp

FORMS += \
    qtestceplibfileiomainwindow.ui
