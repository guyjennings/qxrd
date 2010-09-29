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
