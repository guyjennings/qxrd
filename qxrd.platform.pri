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

win32 {
  contains(QMAKE_HOST.arch,x86_64) {
    WIN64 = 1
    PE_SDK = "c:/XIS/SDK64/"
  } else {
    WIN64 = 0
    PE_SDK = "c:/XIS/SDK32/"
  }

  exists("c:/XIS/SDK/") {
    WIN64 = 0
    PE_SDK = "c:/XIS/SDK/"
  }

  exists($${PE_SDK}/XISL.lib) {
    DEFINES += HAVE_PERKIN_ELMER
    INCLUDEPATH += $${PE_SDK}
    message("Perkin Elmer Software Found")
  }
}

win32 {
  contains(QMAKE_HOST.arch,x86_64) {
    WIN64   = 1
    DEX_SDK = "c:/Program Files/PerkinElmer/DexelaDetectorAPI/"
  } else {
    WIN64   = 0
    DEX_SDK = ""
  }

  exists($${DEX_SDK}/include/BusScanner.h) {
    DEFINES += HAVE_DEXELA
    message("Dexela Detector SDK Found")
  }
}

win32 {
  exists("c:/Program Files/National Instruments/Shared/ExternalCompilerSupport/C/include/NIDAQmx.h") {
    exists("c:/Program Files/National Instruments/Shared/ExternalCompilerSupport/C/lib32/msvc/NIDAQmx.lib") {
      DEFINES += HAVE_NIDAQ
      NIDAQ_HEADER="c:/Program Files/National Instruments/Shared/ExternalCompilerSupport/C/include/"
      NIDAQ_LIBS="c:/Program Files/National Instruments/Shared/ExternalCompilerSupport/C/lib32/msvc/NIDAQmx.lib"
      message("NIDAQ Software Found")
    }
  }

  exists("c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/include/NIDAQmx.h") {
    contains(QMAKE_HOST.arch,x86_64) {
      exists("c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.lib") {
        DEFINES += HAVE_NIDAQ
        NIDAQ_HEADER="c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/include/"
        NIDAQ_LIBS="c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.lib"
        message("64 Bit NIDAQ Software Found")
      }
    } else {
      exists("c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib32/msvc/NIDAQmx.lib") {
        DEFINES += HAVE_NIDAQ
        NIDAQ_HEADER="c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/include/"
        NIDAQ_LIBS="c:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib32/msvc/NIDAQmx.lib"
        message("32 Bit NIDAQ Software Found")
      }
    }
  }
}

!isEmpty(QXRD_PLUGIN_PATH):DEFINES += QXRD_PLUGIN_PATH=\"$$QXRD_PLUGIN_PATH\"
