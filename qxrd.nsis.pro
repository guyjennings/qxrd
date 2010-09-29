include("qxrd.version.pri")
include("qxrd.platform.pri")

TEMPLATE = app
TARGET   = junk_nsis

SOURCES  = \
  junk_nsis.cpp

win32 {
  PRE_TARGETDEPS = nsis

  QMAKE_EXTRA_TARGETS += nsis

  nsis.depends = source plugins app qxrd.nsi
  OUT_PWD_WIN = $${replace(OUT_PWD, /, \)}
  PWD_WIN = $${replace(PWD, /, \)}
  nsis.commands = makensis.exe /V4

  contains(QMAKE_HOST.arch,x86_64) {
    nsis.commands += /DWIN64
  }

  nsis.commands += /DVERSION=$${VERSION} /DPREFIX="\"$${QXRDSUFFIX}\"" /DPREFIXSTR="\"$${QXRDSUFFIXSTR}\"" /DAPPDIR="\"$${OUT_PWD_WIN}\"" $${PWD_WIN}\qxrd.nsi
}
