message("Generate NSIS Installer for libs")

QMAKE_EXTRA_TARGETS += nsis-libs nsis vcredist

OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
PWD_WIN     = $${replace(PWD, /, \\)}

exists("c:/Program Files/NSIS/makensis.exe") {
  NSIS = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
} else:exists("c:/Program Files (x86)/NSIS/makensis.exe") {
  NSIS = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
}

!isEmpty(NSIS) {
  nsis-libs.commands = $${NSIS}

  contains(QMAKE_HOST.arch,x86_64) {
    nsis-libs.commands  += /DWIN64
    nsis-libs.depends   += vcredist
    vcredist.commands = $(COPY_FILE) \"$${PWD_WIN}\\vcredist_vs2017_x64.exe\" \".\"
  } else {
    nsis-libs.depends   += vcredist
    vcredist.commands = $(COPY_FILE) \"$${PWD_WIN}\\vcredist_vs2017_x86.exe\" \".\"
  }

  nsis-libs.commands += \
        /DLIBVERSION=\"$${NSISVERSION}\" \
        /DVERSION=\"$${VERSION}\" \
        /DAPPDIR=\"$${OUT_PWD_WIN}\"

  CONFIG(release, debug|release) {
    nsis-libs.commands += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
  } else {
    nsis-libs.commands += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
  }

  nsis-libs.commands += \"$${PWD_WIN}\\libs.nsi\"
  nsis-libs.depends  += $${PWD}/libs.nsi
  nsis.depends += nsis-libs
}
