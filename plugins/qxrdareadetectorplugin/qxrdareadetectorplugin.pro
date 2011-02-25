include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../app/plugins/

INCLUDEPATH += ../../source/

SOURCES += \
    qxrdareadetectorplugin.cpp

HEADERS += \
    qxrdareadetectorplugin.h
