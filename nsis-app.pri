message("Generate NSIS installer for $${NSISTARGET}")

QMAKE_EXTRA_TARGETS += $${NSISTARGET}-nsis

OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
PWD_WIN     = $${replace(PWD, /, \\)}

include("apps/$${NSISTARGET}-app/$${NSISTARGET}.version.pri")

exists("c:/Program Files/NSIS/makensis.exe") {
  NSIS = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
} else:exists("c:/Program Files (x86)/NSIS/makensis.exe") {
  NSIS = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
}

!isEmpty(NSIS) {
  $${NSISTARGET}-nsis.commands = $${NSIS}

  contains(QMAKE_HOST.arch,x86_64) {
    $${NSISTARGET}-nsis.commands += /DWIN64
  } else {
  }

  $${NSISTARGET}-nsis.commands += \
        /DTARGET=$${NSISTARGET} \
        /DTARGETUC=$$upper($${NSISTARGET}) \
        /DDESCRIPTION=\"$${DESCRIPTION}\" \
        /DLIBVERSION=\"$${NSISVERSION}\" \
        /DVERSION=\"$${VERSION}\" \
        /DAPPDIR=\"$${OUT_PWD_WIN}\"

  CONFIG(release, debug|release) {
    $${NSISTARGET}-nsis.commands += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
  } else {
    $${NSISTARGET}-nsis.commands += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
  }

  $${NSISTARGET}-nsis.commands += \"$${PWD_WIN}\\app.nsi\"
  $${NSISTARGET}-nsis.depends  += $${PWD}/app.nsi
  $${NSISTARGET}-nsis.depends  += vcredist
  nsis.depends                 += $${NSISTARGET}-nsis
}
