QMAKE_EXTRA_TARGETS += app

QTBINDIR = $$[QT_INSTALL_BINS]
QTBASEDIR = $$[QT_INSTALL_PREFIX]

win32 {
    PLATFORM_PREFIX = win32
    exists($${QTBINDIR}) {
        LIBDIR = $${QTBINDIR}
        message("LIBDIR = $${LIBDIR}")
    }
    exists($${LIBDIR}/libgcc_s_dw2-1.dll) {
        message("MINGW found in $${LIBDIR}/libgcc_s_dw2-1.dll")
        qtlibs += $${LIBDIR}/libgcc_s_dw2-1.dll
    }
    exists($${LIBDIR}/mingwm10.dll) {
        message("MINGW found in $${LIBDIR}/mingwm10.dll")
        qtlibs += $${LIBDIR}/mingwm10.dll
    }
    CONFIG(debug, debug|release):qtlibs = $${LIBDIR}/QtCored4.dll \
        $${LIBDIR}/QtNetworkd4.dll \
        $${LIBDIR}/QtGuid4.dll \
        $${LIBDIR}/QtScriptd4.dll
    else:qtlibs = $${LIBDIR}/QtCore4.dll \
        $${LIBDIR}/QtNetwork4.dll \
        $${LIBDIR}/QtGui4.dll \
        $${LIBDIR}/QtScript4.dll
    INCLUDEPATH += .
    INCLUDEPATH += SDK

#  app.CONFIG += recursive

  OUT_PWD_WIN = $${replace(OUT_PWD, /, \)}
  qtlibs_WIN  = $${replace(qtlibs, /, \)}
  app.depends  += FORCE

  app.commands = @echo $${OUT_PWD_WIN}\app

  for(file,qtlibs_WIN) {
    app.commands += \
      && $(COPY_FILE) $${file} $${OUT_PWD_WIN}\app
  }

#  SUBDIRS += app
}

message("Target = $${TARGET} Target Extension = $${TARGET_EXT}")
