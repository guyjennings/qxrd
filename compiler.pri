CONFIG += qt

QT += \
  widgets \
  network \
  qml \
  script \
  concurrent

*g++* {
    QMAKE_CXXFLAGS += -g
    QMAKE_CFLAGS += -g
    QMAKE_LFLAGS += -g
}

MOC_DIR     = moc
UI_DIR      = ui
OBJECTS_DIR = obj
RCC_DIR     = rcc

win32 {
    CONFIG(debug, debug|release):CONFIG += console
}

macx{
  QMAKE_MAC_SDK=macosx10.12
  QXRDSUFFIX = -macx
  QXRDSUFFIXSTR = " mac"
} else:unix {
  contains(QMAKE_HOST.arch,x86_64) {
    QXRDSUFFIX = -linux64
    QXRDSUFFIXSTR = "linux 64"
    QXRD_PLUGIN_PATH = /usr/lib64/qxrd-$${VERSION}/plugins
    QMAKE_LFLAGS += -Wl,-z,defs
  } else {
    QXRDSUFFIX = -linux32
    QXRDSUFFIXSTR = "linux 32"
    QXRD_PLUGIN_PATH = /usr/lib/qxrd-$${VERSION}/plugins
    QMAKE_LFLAGS += -Wl,-z,defs
  }
}

win32-msvc* {
  contains(QMAKE_HOST.arch,x86_64) {
    QXRDSUFFIX = -qt-$${QT_VERSION}-msvc-x64
    QXRDSUFFIXSTR = " QT $${QT_VERSION} (MSVC 64 bit)"
    DEFINES += WIN64
  } else {
    QXRDSUFFIX = -qt-$${QT_VERSION}-msvc-x86
    QXRDSUFFIXSTR = " QT $${QT_VERSION} (MSVC 32 bit)"
  }

  message("Visual C++ Build")
  QMAKE_CXXFLAGS += /Zi
  QMAKE_CFLAGS += /Zi
  QMAKE_LFLAGS += /DEBUG /FIXED:no
  DEFINES += NOMINMAX
}

win32-g++ {
  QXRDSUFFIX = -qt-$${QT_VERSION}-mingw
  QXRDSUFFIXSTR = " QT $${QT_VERSION} (MINGW 32 bit)"
  message("MingW g++ build")
}

!isEmpty(QXRD_PLUGIN_PATH):DEFINES += QXRD_PLUGIN_PATH=\"$$QXRD_PLUGIN_PATH\"

qtHaveModule(datavisualization):qtHaveModule(charts) {
    QT += datavisualization charts
    DEFINES += HAVE_DATAVIS
}

