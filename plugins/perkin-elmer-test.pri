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

