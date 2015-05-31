
QMAKE_EXTRA_TARGETS += nsis

OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
PWD_WIN = $${replace(PWD, /, \\)}

nsis.depends += FORCE

INSTALLS += nsis

win32-g++ {
  exists("c:/Program Files/NSIS/makensis.exe") {
    NSIS = "\"c:/Program Files/NSIS/makensis.exe\"" //V4
  }
  exists("c:/Program Files (x86)/NSIS/makensis.exe") {
    NSIS = "\"c:/Program Files (x86)/NSIS/makensis.exe\"" //V4
  }

  !isEmpty(NSIS) {
    nsis.commands = $${NSIS}
    message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

    contains(QMAKE_HOST.arch,x86_64) {
      nsis.commands += //DWIN64
    }

    nsis.commands += //DVERSION=$${VERSION} //DAPPDIR=\"$${OUT_PWD_WIN}\\.\"

    CONFIG(release, debug|release) {
      nsis.commands += //DPREFIX=\"$${QXRDSUFFIX}\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
    } else {
      nsis.commands += //DPREFIX=\"$${QXRDSUFFIX}-dbg\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
    }

    isEqual(QT_MAJOR_VERSION, 4) {
      nsis.commands += \"$${PWD_WIN}\\qxrd.nsi\"
      nsis.depends  += $${PWD}/qxrd.nsi
    } else {
      nsis.commands += \"$${PWD_WIN}\\qxrd-qt5.nsi\"
      nsis.depends  += $${PWD}/qxrd-qt5.nsi
    }
  }
}

win32-msvc* {
  exists("c:/Program Files/NSIS/makensis.exe") {
    NSIS = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
  }
  exists("c:/Program Files (x86)/NSIS/makensis.exe") {
    NSIS = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
  }

  !isEmpty(NSIS) {
    nsis.commands = $${NSIS}
    message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

    contains(QMAKE_HOST.arch,x86_64) {
      nsis.commands += /DWIN64
    }

    nsis.commands += /DVERSION=$${VERSION} /DAPPDIR=\"$${OUT_PWD_WIN}\\.\"

    CONFIG(release, debug|release) {
      nsis.commands += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
    } else {
      nsis.commands += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
    }

    isEqual(QT_MAJOR_VERSION, 4) {
      nsis.commands += \"$${PWD_WIN}\\qxrd.nsi\"
      nsis.depends  += $${PWD}/qxrd.nsi
    } else {
      nsis.commands += \"$${PWD_WIN}\\qxrd-qt5.nsi\"
      nsis.depends  += $${PWD}/qxrd-qt5.nsi
    }
  }
}
