macx{
  include("dmg.pri")
} else:unix {
  include("rpm.pri")
} else:win32 {
  include("nsis.pri")
}

macx {
  ICON = $${TARGET}-icon.icns
}

win32 {
  RC_FILE = $${TARGET}.rc
}

OTHER_FILES += $${TARGET}-icon.icns $${TARGET}.rc $${TARGET}.nsi
