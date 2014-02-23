TEMPLATE = app
TARGET   = qtestceplib-qwt
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-qwt.pri)

HEADERS += \
    qtestceplibqwtmainwindow.h

SOURCES += \
    qtestceplibqwtmain.cpp \
    qtestceplibqwtmainwindow.cpp

FORMS += \
    qtestceplibqwtmainwindow.ui
