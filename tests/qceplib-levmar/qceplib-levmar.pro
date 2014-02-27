TEMPLATE = app
TARGET   = qtestceplib-levmar
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-levmar.pri)

HEADERS += \
    qtestcepliblevmarmainwindow.h

SOURCES += \
    qtestcepliblevmarmain.cpp \
    qtestcepliblevmarmainwindow.cpp

FORMS += \
    qtestcepliblevmarmainwindow.ui
