TEMPLATE = lib
CONFIG += plugin

win32 {
  contains(QMAKE_HOST.arch,x86_64) {
    PE_SDK = c:/XIS/SDK64/
  } else {
    PE_SDK = c:/XIS/SDK32/
  }

  LIBS += $${PE_SDK}/XISL.lib
  DEFINES += HAVE_PERKIN_ELMER
}

DESTDIR = ../../app/plugins/

INCLUDEPATH += ../../source/
INCLUDEPATH += $$PE_SDK/

HEADERS += \
    qxrdperkinelmerplugin.h

SOURCES += \
    qxrdperkinelmerplugin.cpp
