TEMPLATE = app
TARGET   = qtestceplib-specserver
DESTDIR  = ..

include(../../qceplib.pri)
include(../../qceplib-specserver.pri)

HEADERS += \
    qtestceplibspecservermainwindow.h

SOURCES += \
    qtestceplibspecservermain.cpp \
    qtestceplibspecservermainwindow.cpp

FORMS += \
    qtestceplibspecservermainwindow.ui
