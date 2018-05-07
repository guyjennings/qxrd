win32 {
# Copy QT Libraries into app directory
  LIBDIR = $$[QT_INSTALL_BINS]
  LIBDIR_WIN = $${replace(LIBDIR, /, \\)}
  PWD_WIN    = $${replace(PWD, /, \\)}
  LIBDIR_MINGW = $${replace(LIBDIR, \\\\, /)}

  message(Libdir: $${LIBDIR})
  message(Libdir_win: $${LIBDIR_WIN})
  message(Pwd_win: $${PWD_WIN})
  message(Libdir_mingw: $${LIBDIR_MINGW})
  message($${replace(LIBDIR, \\\\, /)})

  QMAKE_EXTRA_TARGETS += qtlibs

  CONFIG(debug, debug|release) {
    libs =  Qt5Cored \
            Qt5Networkd \
            Qt5Chartsd \
            Qt5DataVisualizationd \
            Qt5Qmld \
            Qt5Testd \
            Qt5Guid \
            Qt5Scriptd \
            Qt5Widgetsd \
            Qt5Svgd \
            Qt5OpenGLd \
            Qt5PrintSupportd \
            Qt5Concurrentd

    platform = qwindowsd

    libs += libEGLd libGLESv2d
  } else {
    libs =  Qt5Core \
            Qt5Network \
            Qt5Charts \
            Qt5DataVisualization \
            Qt5Qml \
            Qt5Test \
            Qt5Gui \
            Qt5Script \
            Qt5Widgets \
            Qt5Svg \
            Qt5OpenGL \
            Qt5PrintSupport \
            Qt5Concurrent

    platform = qwindows

    libs += libEGL libGLESv2
  }

  QMAKE_EXTRA_TARGETS += qtplatformdir
  qtplatformdir.target = platforms
  qtplatformdir.commands = if not exist platforms $(MKDIR) platforms

  QMAKE_EXTRA_TARGETS += qtplatform
  qtplatform.target   = platforms/$${platform}.dll
  qtplatform.depends  = qtplatformdir $${LIBDIR}/../plugins/platforms/$${platform}.dll
  qtplatform.commands += $(COPY_FILE) /Y $${LIBDIR_WIN}\\..\\plugins\\platforms\\$${platform}.dll platforms\\$${platform}.dll &

  qtlibs.depends += qtplatform

  for(lib, libs) {
    !build_pass:message(Target $${lib})
    qtlibs.depends     += $${LIBDIR}/$${lib}.dll
    qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll $${lib}.dll &
  }

  QMAKE_CLEAN += platforms/*
  QMAKE_CLEAN += platforms

  message(Looking for vcredist)

  message(MSVC 2017 VCRedist)

  QMAKE_EXTRA_TARGETS += vcredist

  if (contains(QMAKE_HOST.arch, x86_64)) {
    vcredist.depends  += $${PWD}/vcredist_vs2017_x64.exe
    vcredist.commands += $(COPY_FILE) $${PWD_WIN}\\vcredist_vs2017_x64.exe . &
  } else {
    vcredist.depends  += $${PWD}/vcredist_vs2017_x86.exe
    vcredist.commands += $(COPY_FILE) $${PWD_WIN}\\vcredist_vs2017_x86.exe . &
  }

  QMAKE_CLEAN += *.dll
  QMAKE_CLEAN += plugins/*
  QMAKE_CLEAN += plugins

  PRE_TARGETDEPS  += qtlibs
  PRE_TARGETDEPS  += vcredist
}

$${TARGET}.depends += qtlibs vcredist
