TEMPLATE = app
TARGET   = qxrd
DESTDIR  = ../../

include("../../qxrd.version.pri")
include("../../compiler.pri")
include("../../extras-app.pri")

DEFINES += QXRD_VERSION=\"$$VERSION\"

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

include(../../libraries/qceplib/qceplib/qceplib-qwt-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-base-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-mar345-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-cbf-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-tiff-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-levmar-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-szip-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-zlib-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-hdf5-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-specserver-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-bzip2-include.pri)
##include(../../libraries/qceplib/qceplib/qceplib-nexus.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqxrdlib

INCLUDEPATH += $${PWD}/../../libraries/qxrdlib/
INCLUDEPATH += $${OUT_PWD}/../../libraries/qceplib/ui/
INCLUDEPATH += $${OUT_PWD}/../../libraries/qxrdlib/ui/

#macx {
#}

SOURCES += qxrd.cpp

OTHER_FILES += testImage.js

macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

#never {
## win32 { # Make NSIS installer...
#    OUT_PWD_WIN = $${replace(OUT_PWD, /, \\)}
#    PWD_WIN = $${replace(PWD, /, \\)}

#    exists("c:/Program Files/NSIS/makensis.exe") {
#      win32-g++ {
#        QMAKE_POST_LINK = "\"c:/Program Files/NSIS/makensis.exe\"" //V4
#      } else {
#        QMAKE_POST_LINK = "\"c:\\Program Files\\NSIS\\makensis.exe\"" /V4
#      }
#      message("NSIS found in Program Files")
#    }

#    exists("c:/Program Files (x86)/NSIS/makensis.exe") {
#      win32-g++ {
#        QMAKE_POST_LINK = "\"c:/Program Files (x86)/NSIS/makensis.exe\"" //V4
#      } else {
#        QMAKE_POST_LINK = "\"c:\\Program Files (x86)\\NSIS\\makensis.exe\"" /V4
#      }
#      message("NSIS found in Program Files (x86)")
#    }

#    !isEmpty(QMAKE_POST_LINK) {
#      contains(QMAKE_HOST.arch,x86_64) {
#        win32-g++ {
#          QMAKE_POST_LINK += //DWIN64
#        } else {
#          QMAKE_POST_LINK += /DWIN64
#        }
#      }

#      message("Generate NSIS installer for $${QXRDSUFFIXSTR}")

#      win32-g++ {
#        QMAKE_POST_LINK += //DVERSION=$${VERSION} //DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
#      } else {
#        QMAKE_POST_LINK += /DVERSION=$${VERSION} /DAPPDIR=\"$${OUT_PWD_WIN}\\..\\.\"
#      }

#      CONFIG(release, debug|release) {
#        win32-g++ {
#          QMAKE_POST_LINK += //DPREFIX=\"$${QXRDSUFFIX}\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
#        } else {
#          QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR}\"
#        }
#      } else {
#        win32-g++ {
#          QMAKE_POST_LINK += //DPREFIX=\"$${QXRDSUFFIX}-dbg\" //DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
#        } else {
#          QMAKE_POST_LINK += /DPREFIX=\"$${QXRDSUFFIX}-dbg\" /DPREFIXSTR=\"$${QXRDSUFFIXSTR} Debug\"
#        }
#      }

#      isEqual(QT_MAJOR_VERSION, 4) {
#        QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd.nsi\"
#      } else {
#        QMAKE_POST_LINK += \"$${PWD_WIN}\\..\\qxrd-qt5.nsi\"
#      }
#   }
#}

INCLUDEPATH += $$PWD/../../libraries/qceplib
DEPENDPATH += $$PWD/../../libraries/qceplib

INCLUDEPATH += $$OUT_PWD/../../libraries/qceplib
DEPENDPATH  += $$OUT_PWD/../../libraries/qceplib

INCLUDEPATH += $$PWD/../../libraries/qxrdlib
DEPENDPATH += $$PWD/../../libraries/qxrdlib

INCLUDEPATH += $$OUT_PWD/../../libraries/qxrdlib
DEPENDPATH  += $$OUT_PWD/../../libraries/qxrdlib
