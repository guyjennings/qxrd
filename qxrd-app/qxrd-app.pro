TEMPLATE = app
TARGET   = qxrd
DESTDIR  = ../

include("../qxrd.version.pri")
include("../qxrd.platform.pri")

DEFINES += QXRD_VERSION=\"$$VERSION\"

message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Destdir = $${DESTDIR})

OTHER_FILES += \
    qxrd.rc \
    simpleServerTest.mac \
    testScripts.js \
    testScriptPlot.js \
    testImage.js \
    tilt_test.js \
    testscript.js \
    racetest.js \
    racetest.mac \
    unsubtractDark.js \
    qxrdexampleusergeometry.js \
    qxrdexampleuserabsorption.js \
    qxrd_macrocounter.mac \
    calgrid.js \
    findring.js \
    dumpRings.js

DISTFILES += qxrd.dox \
    download/index.php \
    tiffconfig/win32/tif_config.h \
    tiffconfig/win32/tiffconf.h \
    tiffconfig/macx/tif_config.h \
    tiffconfig/macx/tiffconf.h \
    qxrd_tests.mac

CONFIG += qceplib-qwt
CONFIG += qceplib-hdf5
CONFIG += qceplib-tiff
CONFIG += qceplib-bzip2

#CONFIG += qceplib-nexus

include(../libraries/qceplib/qceplib/qceplib-qwt-include.pri)
include(../libraries/qceplib/qceplib/qceplib-base-include.pri)
include(../libraries/qceplib/qceplib/qceplib-mar345-include.pri)
include(../libraries/qceplib/qceplib/qceplib-cbf-include.pri)
include(../libraries/qceplib/qceplib/qceplib-tiff-include.pri)
include(../libraries/qceplib/qceplib/qceplib-levmar-include.pri)
include(../libraries/qceplib/qceplib/qceplib-szip-include.pri)
include(../libraries/qceplib/qceplib/qceplib-zlib-include.pri)
include(../libraries/qceplib/qceplib/qceplib-hdf5-include.pri)
include(../libraries/qceplib/qceplib/qceplib-specserver-include.pri)
include(../libraries/qceplib/qceplib/qceplib-bzip2-include.pri)
##include(../libraries/qceplib/qceplib/qceplib-nexus.pri)

INCLUDEPATH += $${PWD}/../libraries/qxrdlib/
INCLUDEPATH += $${OUT_PWD}/../libraries/qxrdlib/

#macx {
#}

SOURCES += qxrd.cpp

OTHER_FILES += testImage.js

macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

OTHER_FILES += qxrd.rc \
    qxrd.nsi \
    qxrd-qt5.nsi \
    HeaderTemplates.txt \
    vcredist_vs2013_x86.exe \
    vcredist_vs2013_x64.exe \
    vcredist_vs2017_x86.exe \
    vcredist_vs2017_x64.exe

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
  qtplatformdir.target = ../platforms
  qtplatformdir.commands = if not exist ..\\platforms $(MKDIR) ..\\platforms

  QMAKE_EXTRA_TARGETS += qtplatform
  qtplatform.target   = ../platforms/$${platform}.dll
  qtplatform.depends  = qtplatformdir $${LIBDIR}/../plugins/platforms/$${platform}.dll
  qtplatform.commands += $(COPY_FILE) /Y $${LIBDIR_WIN}\\..\\plugins\\platforms\\$${platform}.dll ..\\platforms\\$${platform}.dll &

  qtlibs.depends += qtplatform

  for(lib, libs) {
    !build_pass:message(Target $${lib})
    qtlibs.depends     += $${LIBDIR}/$${lib}.dll
    qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll ..\\$${lib}.dll &
  }

  QMAKE_CLEAN += ../platforms/*
  QMAKE_CLEAN += ../platforms

  message(Looking for vcredist)

  message(MSVC 2017 VCRedist)

  QMAKE_EXTRA_TARGETS += vcredist

  if (contains(QMAKE_HOST.arch, x86_64)) {
    vcredist.depends  += $${PWD}/../vcredist_vs2017_x64.exe
    vcredist.commands += $(COPY_FILE) $${PWD_WIN}\\..\\vcredist_vs2017_x64.exe .. &
  } else {
    vcredist.depends  += $${PWD}/../vcredist_vs2017_x86.exe
    vcredist.commands += $(COPY_FILE) $${PWD_WIN}\\..\\vcredist_vs2017_x86.exe .. &
  }

  QMAKE_CLEAN += ../*.dll
  QMAKE_CLEAN += ../plugins/*
  QMAKE_CLEAN += ../plugins

  PRE_TARGETDEPS  += qtlibs
  PRE_TARGETDEPS  += vcredist
}

TARGET.depends += qtlibs vcredist

never {
# win32 { # Make NSIS installer...
    OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
    PWD_WIN = $${replace(PWD, /, \\)}

    exists("c:/Program Files/NSIS/makensis.exe") {
      win32-g++ {
        QMAKE_POST_LINK = "\"c:/Program Files/NSIS/makensis.exe\"" //V4
      } else {
        QMAKE_POST_LINK = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
      }
      message("NSIS found in Program Files")
    }

    exists("c:/Program Files (x86)/NSIS/makensis.exe") {
      win32-g++ {
        QMAKE_POST_LINK = "\"c:/Program Files (x86)/NSIS/makensis.exe\"" //V4
      } else {
        QMAKE_POST_LINK = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
      }
      message("NSIS found in Program Files (x86)")
    }

    !isEmpty(QMAKE_POST_LINK) {
      contains(QMAKE_HOST.arch,x86_64) {
        win32-g++ {
          QMAKE_POST_LINK += //DWIN64
        } else {
          QMAKE_POST_LINK += /DWIN64
        }
      }

      message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

      win32-g++ {
        QMAKE_POST_LINK += //DVERSION=$${VERSION} //DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
      } else {
        QMAKE_POST_LINK += /DVERSION=$${VERSION} /DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
      }

      CONFIG(release, debug|release) {
        win32-g++ {
          QMAKE_POST_LINK += //DPREFIX=\"$${QXRDSUFFIX}\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
        } else {
          QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
        }
      } else {
        win32-g++ {
          QMAKE_POST_LINK += //DPREFIX=\"$${QXRDSUFFIX}-dbg\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
        } else {
          QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
        }
      }

      isEqual(QT_MAJOR_VERSION, 4) {
        QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd.nsi\"
      } else {
        QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd-qt5.nsi\"
      }
   }
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../ -lqceplib

INCLUDEPATH += $$PWD/../libraries/qceplib
DEPENDPATH += $$PWD/../libraries/qceplib

INCLUDEPATH += $$OUT_PWD/../libraries/qceplib
DEPENDPATH  += $$OUT_PWD/../libraries/qceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../ -lqxrdlib

INCLUDEPATH += $$PWD/../libraries/qxrdlib
DEPENDPATH += $$PWD/../libraries/qxrdlib

INCLUDEPATH += $$OUT_PWD/../libraries/qxrdlib
DEPENDPATH  += $$OUT_PWD/../libraries/qxrdlib
