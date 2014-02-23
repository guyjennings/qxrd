TEMPLATE = app
TARGET   = qtestceplib
DESTDIR  = ..

include(../../qceplib.pri)

HEADERS += \
    qtestceplibmainwindow.h

SOURCES += \
    qtestceplibmain.cpp \
    qtestceplibmainwindow.cpp

FORMS += \
    qtestceplibmainwindow.ui
