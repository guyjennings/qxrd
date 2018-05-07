win32 {
  exists("c:/Program Files/Allied Vision/Vimba_2.1/VimbaCPP/Include/VimbaSystem.h") {
    DEFINES += HAVE_ALLIEDVISION
    VIMBA_HEADER    = "c:/Program Files/Allied Vision/Vimba_2.1/"
    VIMBA_CHEADER   = "c:/Program Files/Allied Vision/Vimba_2.1/VimbaC/Include/"
    VIMBA_CPPHEADER = "c:/Program Files/Allied Vision/Vimba_2.1/VimbaCPP/Include/"
    contains(QMAKE_HOST.arch,x86_64) {
      VIMBA_CLIBS     = "c:/Program Files/Allied Vision/Vimba_2.1/VimbaC/Lib/Win64/"
      VIMBA_CPPLIBS   = "c:/Program Files/Allied Vision/Vimba_2.1/VimbaCPP/Lib/Win64/"
      message("64 Bit Vimba 2.1 Found")
    } else {
      VIMBA_CLIBS     = "c:/Program Files/Allied Vision/Vimba_2.1/VimbaC/Lib/Win32/"
      VIMBA_CPPLIBS   = "c:/Program Files/Allied Vision/Vimba_2.1/VimbaCPP/Lib/Win32/"
      message("32 Bit Vimba 2.1 Found")
    }
  }
}

macx {
# look for vimba on Mac OS X
} else:unix {
# look for vimba on Linux
}

