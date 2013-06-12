CONFIG   += qt
QT       += core gui script network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qceplib
TEMPLATE = app
QCEPLIB = ./

include("qceplib.pri")

SOURCES += main.cpp

OTHER_FILES += \
    qceplib.pri

win32 {
#    RC_FILE = qxrd.rc
    CONFIG(debug, debug|release):CONFIG += console
}
