include("../qxrd.version.pri")
include("../qxrd.platform.pri")

message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Destdir = $${DESTDIR})

TEMPLATE = app
TARGET   = qxrd
DESTDIR  = ../
#DESTDIR  = ../app/

macx {
  ICON = images/qxrd-icon.icns
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent datavisualization charts

CONFIG += qt

QT += network \
    qml \
    script \
    scripttools \
    testlib

unix {
    message(Version = $${VERSION})
    message(Plugin path = $${QXRD_PLUGIN_PATH})
}

DEFINES += QXRD_VERSION=\"$$VERSION\"

!isEmpty(QXRD_PLUGIN_PATH):DEFINES += QXRD_PLUGIN_PATH=\"$$QXRD_PLUGIN_PATH\"

*g++* { 
    QMAKE_CXXFLAGS += -g
    QMAKE_CFLAGS += -g
    QMAKE_LFLAGS += -g
}

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc
message(Building: $${TARGET})
win32 { 
    RC_FILE = qxrd.rc
    CONFIG(debug, debug|release):CONFIG += console
}

INCLUDEPATH += qceplib

INCLUDEPATH += $${PWD}/../libraries/qxrdlib/
INCLUDEPATH += $${OUT_PWD}/../libraries/qxrdlib/

OTHER_FILES += \
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

CONFIG += qceplib-qwt qceplib-hdf5 qceplib-tiff

#CONFIG += qceplib-nexus

include(submodules/qceplib/qceplib-qwt-include.pri)
include(submodules/qceplib/qceplib-base-include.pri)
include(submodules/qceplib/qceplib-mar345-include.pri)
include(submodules/qceplib/qceplib-cbf-include.pri)
include(submodules/qceplib/qceplib-tiff-include.pri)
include(submodules/qceplib/qceplib-levmar-include.pri)
include(submodules/qceplib/qceplib-szip-include.pri)
include(submodules/qceplib/qceplib-zlib-include.pri)
include(submodules/qceplib/qceplib-hdf5-include.pri)
include(submodules/qceplib/qceplib-specserver-include.pri)
##include(submodules/qceplib/qceplib-nexus.pri)

#macx {
#}


SOURCES += qxrd.cpp

OTHER_FILES += testImage.js

macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

contains(DEFINES,HAVE_PERKIN_ELMER) {
  SOURCES += \
             qxrdperkinelmerplugininterface.cpp
  HEADERS += \
             qxrdperkinelmerplugininterface.h \
             qxrdperkinelmerplugininterface-ptr.h
}

OTHER_FILES += qxrd.rc \
    qxrd.nsi \
    qxrd-qt5.nsi \
    qxrd-cuda.pri \
    HeaderTemplates.txt

win32 {
# Copy QT Libraries into app directory
  LIBDIR = $$[QT_INSTALL_BINS]
  LIBDIR_WIN = $${replace(LIBDIR, /, \\)}
  LIBDIR_MINGW = $${replace(LIBDIR, \\\\, /)}

  message(Libdir: $${LIBDIR})
  message(Libdir_win: $${LIBDIR_WIN})
  message(Libdir_mingw: $${LIBDIR_MINGW})
  message($${replace(LIBDIR, \\\\, /)})

  QMAKE_EXTRA_TARGETS += qtlibs

  isEqual(QT_MAJOR_VERSION, 5) {
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

      win32-msvc* {
        libs += libEGLd libGLESv2d
      }
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

      win32-msvc* {
        libs += libEGL libGLESv2
      }
    }

    win32-msvc2017 {
    } else:win32-msvc2013 {
      greaterThan(QT_MINOR_VERSION, 6) {
      } else {
        greaterThan(QT_MINOR_VERSION, 4) {
          libs += icudt54 icuin54 icuuc54
        } else {
          libs += icudt53 icuin53 icuuc53
        }
      }
    } else:win32-msvc* {
      libs += icudt51 icuin51 icuuc51
    } else:win32-g++ {
      libs += libgcc_s_dw2-1 libwinpthread-1 libstdc++-6 icudt52 icuin52 icuuc52
    }

    QMAKE_EXTRA_TARGETS += qtplatformdir
    qtplatformdir.target = ../platforms

    win32-msvc* {
      qtplatformdir.commands = if not exist ..\\platforms $(MKDIR) ..\\platforms
    } else:win32-g++ {
      qtplatformdir.commands = $(MKDIR) -p ../platforms
    }

    QMAKE_EXTRA_TARGETS += qtplatform
    qtplatform.target   = ../platforms/$${platform}.dll
    qtplatform.depends  = qtplatformdir $${LIBDIR}/../plugins/platforms/$${platform}.dll
    win32-g++ {
      qtplatform.commands += $(COPY_FILE) $${LIBDIR_MINGW}/../plugins/platforms/$${platform}.dll ../platforms/$${platform}.dll
    } else {
      qtplatform.commands += $(COPY_FILE) /Y $${LIBDIR_WIN}\\..\\plugins\\platforms\\$${platform}.dll ..\\platforms\\$${platform}.dll
    }

    qtlibs.depends += qtplatform

    for(lib, libs) {
      !build_pass:message(Target $${lib})
      qtlibs.depends     += $${LIBDIR}/$${lib}.dll
      win32-g++ {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_MINGW}/$${lib}.dll ../$${lib}.dll &
      } else {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll ..\\$${lib}.dll &
      }
    }

    QMAKE_CLEAN += ../platforms/*
    QMAKE_CLEAN += ../platforms
  }

  isEqual(QT_MAJOR_VERSION, 4) {
    CONFIG(debug, debug|release) {
      libs = QtCored4 QtNetworkd4 QtGuid4 QtScriptd4 QtOpenGLd4 QtSvgd4
    } else {
      libs = QtCore4 QtNetwork4 QtGui4 QtScript4 QtOpenGL4 QtSvg4
    }

    win32-g++ {
      libs += libgcc_s_dw2-1 libwinpthread-1 libstdc++-6
    }

    for(lib, libs) {
      !build_pass:message(Target $${lib})

      qtlibs.depends     += $${LIBDIR}/$${lib}.dll
      win32-g++ {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_MINGW}/$${lib}.dll ../$${lib}.dll &
      } else {
        qtlibs.commands    += $(COPY_FILE) $${LIBDIR_WIN}\\$${lib}.dll ..\\$${lib}.dll &
      }
    }
  }

  QMAKE_CLEAN += ../*.dll
  QMAKE_CLEAN += ../plugins/*
  QMAKE_CLEAN += ../plugins

  PRE_TARGETDEPS  += qtlibs
}

TARGET.depends += qtlibs

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
