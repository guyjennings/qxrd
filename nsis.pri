
QMAKE_EXTRA_TARGETS += nsis

OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
PWD_WIN = $${replace(PWD, /, \\)}

setup.depends += qxrd.exe

CONFIG(release, debug|release) {
  setup.target   = qxrd-setup-$${VERSION}$${QXRDSUFFIX}.exe
} else {
  setup.target   = qxrd-setup-$${VERSION}$${QXRDSUFFIX}-dbg.exe
}

#INSTALLS += nsis.target

nsis.depends += $${setup.target}

QMAKE_EXTRA_TARGETS += setup

win32-g++ {
  exists("c:/Program Files/NSIS/makensis.exe") {
    NSIS = "\"c:/Program Files/NSIS/makensis.exe\"" //V4
  }
  exists("c:/Program Files (x86)/NSIS/makensis.exe") {
    NSIS = "\"c:/Program Files (x86)/NSIS/makensis.exe\"" //V4
  }

  !isEmpty(NSIS) {
    setup.commands = $${NSIS}
    message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

    contains(QMAKE_HOST.arch,x86_64) {
      setup.commands += //DWIN64
    }

    setup.commands += //DVERSION=$${VERSION} //DAPPDIR=\"$${OUT_PWD_WIN}\\.\"

    CONFIG(release, debug|release) {
      setup.commands += //DPREFIX=\"$${QXRDSUFFIX}\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
    } else {
      setup.commands += //DPREFIX=\"$${QXRDSUFFIX}-dbg\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
    }

    isEqual(QT_MAJOR_VERSION, 4) {
      setup.commands += \"$${PWD_WIN}\\qxrd.nsi\"
      setup.depends  += $${PWD}/qxrd.nsi
    } else {
      setup.commands += \"$${PWD_WIN}\\qxrd-qt5.nsi\"
      setup.depends  += $${PWD}/qxrd-qt5.nsi
    }
  }
}

win32-msvc* {
  QMAKE_EXTRA_TARGETS += vcredist_x86 vcredist_x64

  vcredist_x86.commands = $(COPY_FILE) \"$${PWD_WIN}\\vcredist_vs2013_x86.exe\" \"$${OUT_PWD_WIN}\\.\"
  vcredist_x64.commands = $(COPY_FILE) \"$${PWD_WIN}\\vcredist_vs2013_x64.exe\" \"$${OUT_PWD_WIN}\\.\"

  exists("c:/Program Files/NSIS/makensis.exe") {
    NSIS = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
  }
  exists("c:/Program Files (x86)/NSIS/makensis.exe") {
    NSIS = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
  }

  !isEmpty(NSIS) {
    setup.commands = $${NSIS}
    message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

    contains(QMAKE_HOST.arch,x86_64) {
      setup.commands += /DWIN64
      setup.depends  += vcredist_x64
    } else {
      setup.depends  += vcredist_x86
    }

    setup.commands += /DVERSION=$${VERSION} /DAPPDIR=\"$${OUT_PWD_WIN}\\.\"

    CONFIG(release, debug|release) {
      setup.commands += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
    } else {
      setup.commands += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
    }

    isEqual(QT_MAJOR_VERSION, 4) {
      setup.commands += \"$${PWD_WIN}\\qxrd.nsi\"
      setup.depends  += $${PWD}/qxrd.nsi
    } else {
      setup.commands += \"$${PWD_WIN}\\qxrd-qt5.nsi\"
      setup.depends  += $${PWD}/qxrd-qt5.nsi
    }
  }
}
