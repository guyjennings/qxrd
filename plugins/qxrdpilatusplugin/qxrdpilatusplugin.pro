include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

INCLUDEPATH += ../../source/

HEADERS += \
    qxrdpilatusplugin.h

SOURCES += \
    qxrdpilatusplugin.cpp
