include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

INCLUDEPATH += ../../source/ ../../source/qceplib/

contains(DEFINES,HAVE_NIDAQ) {
  INCLUDEPATH += "$${NIDAQ_HEADER}"
  LIBS        += "$${NIDAQ_LIBS}"
}

HEADERS += \
    qxrdnidaqplugin.h

SOURCES += \
    qxrdnidaqplugin.cpp
