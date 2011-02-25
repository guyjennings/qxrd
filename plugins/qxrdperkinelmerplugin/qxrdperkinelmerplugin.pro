include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../app/plugins/

INCLUDEPATH += ../../source/
INCLUDEPATH += $$PE_SDK/

LIBS += $${PE_SDK}/XISL.lib

HEADERS += \
    qxrdperkinelmerplugin.h

SOURCES += \
    qxrdperkinelmerplugin.cpp
