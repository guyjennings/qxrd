include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../app/plugins/

INCLUDEPATH += ../../source/

contains(DEFINES,HAVE_NIDAQ) {
  message("INCLUDEPATH += $${NIDAQ_BASE}/include")
  INCLUDEPATH += "$${NIDAQ_BASE}/include"
}

HEADERS += \
    qxrdnidaqplugin.h

SOURCES += \
    qxrdnidaqplugin.cpp
