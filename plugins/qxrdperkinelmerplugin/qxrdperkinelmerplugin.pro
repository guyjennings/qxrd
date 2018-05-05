include("../../qxrd.version.pri")
#include("../../qxrd.platform.pri")

QT += widgets qml network script concurrent

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

include("../../libraries/qceplib/qceplib/qceplib-base-include.pri")
include("../../libraries/qceplib/qceplib/qceplib-specserver-include.pri")
INCLUDEPATH += ../../libraries/qxrdlib/
INCLUDEPATH += $$PE_SDK/

LIBS += $${PE_SDK}/XISL.lib

HEADERS += \
    qxrdperkinelmerplugin.h \
    qxrdperkinelmerdriver.h \
    qxrdperkinelmerdriver-ptr.h \
    qxrdperkinelmerplugin-ptr.h

SOURCES += \
    qxrdperkinelmerplugin.cpp \
    qxrdperkinelmerdriver.cpp

OTHER_FILES += \
    pe.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib

INCLUDEPATH += $$PWD/../../libraries/qceplib
DEPENDPATH += $$PWD/../../libraries/qceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqxrdlib

INCLUDEPATH += $$PWD/../../libraries/qxrdlib
DEPENDPATH += $$PWD/../../libraries/qxrdlib

macx {
  dummyTarget.target   = dmg
  QMAKE_EXTRA_TARGETS += dummyTarget
}
