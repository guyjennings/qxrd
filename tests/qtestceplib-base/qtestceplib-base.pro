TEMPLATE = app
TARGET   = qtestceplib-base
DESTDIR  = ../bin/

greaterThan(QT_MAJOR_VERSION,4): QT += widgets concurrent

include(../../qceplib-base.pri)

HEADERS += \
    qtestceplibmainwindow.h

SOURCES += \
    qtestceplibmain.cpp \
    qtestceplibmainwindow.cpp

FORMS += \
    qtestceplibmainwindow.ui
