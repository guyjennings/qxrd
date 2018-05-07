include("../../qxrd.version.pri")
include("../../compiler.pri")
include("../../extras-lib.pri")
include("../stellarnet-test.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

include("../../libraries/qceplib/qceplib/qceplib-base-include.pri")
include("../../libraries/qceplib/qceplib/qceplib-specserver-include.pri")
INCLUDEPATH += ../../libraries/qxrdlib/

SOURCES += \
  StellarNet.cpp \
  qstellarnetdriver.cpp \
  qstellarnetplugin.cpp

HEADERS += \
  StellarNet.h \
  qstellarnetdriver.h \
  qstellarnetplugin.h \
  qstellarnetplugin-ptr.h \
  qstellarnetdriver-ptr.h

OTHER_FILES += \
    stellarnet.json

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