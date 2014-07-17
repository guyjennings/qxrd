TEMPLATE = app
TARGET   = qtestceplib-base
DESTDIR  = ../bin/

greaterThan(QT_MAJOR_VERSION,4): QT += widgets concurrent

include(../../qceplib-base.pri)

HEADERS += \
    qtestceplibmainwindow.h \
    qtestimagedata.h

SOURCES += \
    qtestceplibmain.cpp \
    qtestceplibmainwindow.cpp \
    qtestimagedata.cpp

FORMS += \
    qtestceplibmainwindow.ui
