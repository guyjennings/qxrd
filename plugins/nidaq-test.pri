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

