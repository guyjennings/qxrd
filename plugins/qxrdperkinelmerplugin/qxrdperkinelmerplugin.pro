include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../app/plugins/

INCLUDEPATH += ../../source/
INCLUDEPATH += $$PE_SDK/

HEADERS += \
    qxrdperkinelmerplugin.h

SOURCES += \
    qxrdperkinelmerplugin.cpp
