include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

INCLUDEPATH += ../../source/
INCLUDEPATH += $$PE_SDK/

LIBS += $${PE_SDK}/XISL.lib

HEADERS += \
    qxrdperkinelmerplugin.h

SOURCES += \
    qxrdperkinelmerplugin.cpp

OTHER_FILES += \
    pe.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../libs/ -lqceplib

INCLUDEPATH += $$PWD/../../libraries/qceplib
DEPENDPATH += $$PWD/../../libraries/qceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../libs/ -lqxrdlib

INCLUDEPATH += $$PWD/../../libraries/qxrdlib
DEPENDPATH += $$PWD/../../libraries/qxrdlib
