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
