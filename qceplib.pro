CONFIG   += qt
QT       += core gui script network opengl svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qceplib
TEMPLATE = app
QCEPLIB = ./

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc

include("qwt-6.1.pri")
include("qceplib.pri")
include("qt-hdf5.pri")

macx:win32 {
  include("qt-nexus.pri")
  DEFINES += HAVE_NEXUS
}

SOURCES += \
    qtestceplibmainwindow.cpp \
    qtestceplibmain.cpp

OTHER_FILES += \
    qceplib.pri \
    qwt-6.1.pri \
    qt-hdf5.pri \
    qt-nexus.pri \
    qt-cbf.pri

win32 {
#    RC_FILE = qxrd.rc
    CONFIG(debug, debug|release):CONFIG += console
}

HEADERS += \
    qtestceplibmainwindow.h

FORMS += \
    qtestceplibmainwindow.ui

