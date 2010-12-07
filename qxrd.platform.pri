macx{
  QXRDSUFFIX = -macx
  QXRDSUFFIXSTR = " mac"
} else:unix {
  contains(QMAKE_HOST.arch,x86_64) {
    QXRDSUFFIX = -linux64
    QXRDSUFFIXSTR = "linux 64"
    QXRD_PLUGIN_PATH = /usr/lib64/qxrd-$${VERSION}/plugins
  } else {
    QXRDSUFFIX = -linux32
    QXRDSUFFIXSTR = "linux 32"
    QXRD_PLUGIN_PATH = /usr/lib/qxrd-$${VERSION}/plugins
  }
}

win32-msvc* {
  contains(QMAKE_HOST.arch,x86_64) {
    QXRDSUFFIX = -msvc-x64
    QXRDSUFFIXSTR = " (MSVC 64 bit)"
  } else {
    QXRDSUFFIX = -msvc-x86
    QXRDSUFFIXSTR = " (MSVC 32 bit)"
  }

  message("Visual C++ Build")
  QMAKE_CXXFLAGS += /Zi
  QMAKE_CFLAGS += /Zi
  QMAKE_LFLAGS += /DEBUG /FIXED:no
}

win32-g++ {
  QXRDSUFFIX = -mingw
  QXRDSUFFIXSTR = " (MINGW 32 bit)"
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
    WIND64 = 0
    PE_SDK = "c:/XIS/SDK/"
  }

  exists($${PE_SDK}/XISL.lib) {
    DEFINES += HAVE_PERKIN_ELMER
    INCLUDEPATH += $${PE_SDK}
    message("Perkin Elmer Software Found")
  }
}

win32 {
  exists("c:/Program Files/National Instruments/NI-DAQ/DAQmx ANSI C Dev/include/NIDAQmx.h") {
    DEFINES += HAVE_NIDAQ
    NIDAQ_BASE="c:/Program Files/National Instruments/NI-DAQ/DAQmx ANSI C Dev/"
    message("NIDAQ Software Found")
  }
}
