TEMPLATE = app
TARGET   = qsw
DESTDIR  = ../../

include("qsw.version.pri")
include("../../compiler.pri")
include("../../extras-app.pri")

DEFINES += QSW_VERSION=\"$$VERSION\"

CONFIG += qceplib-qwt
CONFIG += qceplib-hdf5
CONFIG += qceplib-tiff

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
include(../../libraries/qceplib/qceplib/qceplib-bzip2-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-specserver-include.pri)

INCLUDEPATH += $${PWD}/../../libraries/qxrdlib/
INCLUDEPATH += $${OUT_PWD}/../../libraries/qceplib/ui/
INCLUDEPATH += $${OUT_PWD}/../../libraries/qxrdlib/ui/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqxrdlib

SOURCES += \
  $$PWD/qsw.cpp \
  $$PWD/qswdebug.cpp \
  $$PWD/qswapplication.cpp \
  $$PWD/qswmainwindow.cpp \
  $$PWD/qswexperimentthread.cpp \
  $$PWD/qswexperiment.cpp \
  $$PWD/qswscriptenginethread.cpp \
  $$PWD/qswscriptengine.cpp \
  $$PWD/qswglobalsettings.cpp
  $$PWD/qsw-app.cpp

HEADERS += \
  $$PWD/qswapplication.h \
  $$PWD/qswlib_global.h \
  $$PWD/qswapplication-ptr.h \
  $$PWD/qswdebug.h \
  $$PWD/qswmainwindow.h \
  $$PWD/qswmainwindow-ptr.h \
  $$PWD/qswexperimentthread.h \
  $$PWD/qswexperiment.h \
  $$PWD/qswexperimentthread-ptr.h \
  $$PWD/qswexperiment-ptr.h \
  $$PWD/qswscriptenginethread.h \
  $$PWD/qswscriptengine.h \
  $$PWD/qswscriptengine-ptr.h \
  $$PWD/qswscriptenginethread-ptr.h \
  $$PWD/qswglobalsettings.h \
  $$PWD/qswglobalsettings-ptr.h

FORMS += \
  $$PWD/qswmainwindow.ui

RESOURCES += \
  $$PWD/qsw.qrc
