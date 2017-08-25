include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

INCLUDEPATH += ../../source/

HEADERS += \
    qxrdpilatusplugin.h

SOURCES += \
    qxrdpilatusplugin.cpp

OTHER_FILES += \
    pilatus.json

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../libs/ -lqxrdlib

INCLUDEPATH += $$PWD/../../libraries/qxrdlib
DEPENDPATH += $$PWD/../../libraries/qxrdlib
