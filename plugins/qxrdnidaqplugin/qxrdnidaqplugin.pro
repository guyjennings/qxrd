include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

INCLUDEPATH += ../../source/ ../../source/qwt-5.2/src

contains(DEFINES,HAVE_NIDAQ) {
  INCLUDEPATH += "$${NIDAQ_HEADER}"
  LIBS        += "$${NIDAQ_LIBS}"
}

HEADERS += \
    qxrdnidaqplugin.h

SOURCES += \
    qxrdnidaqplugin.cpp
