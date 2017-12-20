include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

include("../../source/submodules/qceplib/qceplib-base-include.pri")
INCLUDEPATH += ../../libraries/qxrdlib/
INCLUDEPATH += "$${DEX_SDK}/include"
DEPENDPATH  += "$${DEX_SDK}/include"

SOURCES += \
    qxrddexeladriver.cpp \
    qxrddexelaplugin.cpp

HEADERS += \
    qxrddexeladriver.h \
    qxrddexelaplugin.h

OTHER_FILES += \
    dexela.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib

#INCLUDEPATH += $$PWD/../../libraries/qceplib
#DEPENDPATH += $$PWD/../../libraries/qceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqxrdlib

win32{
    contains(QMAKE_HOST.arch,x86_64) {
        LIBS += -L"$${DEX_SDK}/lib/x64/"
    } else {
        LIBS += -L"$${DEX_SDK}/lib/Win32/"
    }
}
