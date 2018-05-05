include("../../qxrd.version.pri")
#include("../../qxrd.platform.pri")

QT += widgets qml network script concurrent

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

include("../../libraries/qceplib/qceplib/qceplib-base-include.pri")
include("../../libraries/qceplib/qceplib/qceplib-specserver-include.pri")
INCLUDEPATH += ../../libraries/qxrdlib/
INCLUDEPATH += "$${VIMBA_HEADER}"
DEPENDPATH  += "$${VIMBA_HEADER}"

SOURCES += \
    qxrdalliedvisiondriver.cpp \
    qxrdalliedvisionplugin.cpp

HEADERS += \
    qxrdalliedvisiondriver.h \
    qxrdalliedvisionplugin.h \
    qxrdalliedvisionplugin-ptr.h \
    qxrdalliedvisiondriver-ptr.h

OTHER_FILES += \
    alliedvision.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib

#INCLUDEPATH += $$PWD/../../libraries/qceplib
#DEPENDPATH += $$PWD/../../libraries/qceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqxrdlib

#INCLUDEPATH += $$PWD/../../libraries/qxrdlib
#DEPENDPATH += $$PWD/../../libraries/qxrdlib

win32{
  CONFIG(release, debug|release) {
    LIBS += -L$${VIMBA_HEADER}/VimbaC/Lib/Win64/ -lVimbaC -L$${VIMBA_HEADER}/VimbaCPP/Lib/Win64/ -lVimbaCPP
  } else {
    LIBS += -L$${VIMBA_HEADER}/VimbaC/Lib/Win64/ -lVimbaC -L$${VIMBA_HEADER}/VimbaCPP/Lib/Win64/ -lVimbaCPP
  }
}

macx {
  dummyTarget.target   = dmg
  QMAKE_EXTRA_TARGETS += dummyTarget
}
