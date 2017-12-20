include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins/

include("../../source/submodules/qceplib/qceplib-base-include.pri")
INCLUDEPATH += ../../libraries/qxrdlib/

SOURCES += \
    qxrddexeladriver.cpp \
    qxrddexelaplugin.cpp

HEADERS += \
    BusScanner.h \
    Dex*.h \
    boost/*.hpp \
    boost/config/*.hpp \
    boost/config/abi/*.hpp \
    boost/config/compiler/*.hpp \
    boost/config/no_tr1/*.hpp \
    boost/config/platform/*.hpp \
    boost/config/stdlib/*.hpp \
    boost/detail/*.hpp \
    boost/detail/*.ipp \
    boost/detail/winapi/*.hpp \
    boost/exception/*.hpp \
    boost/exception/detail/*.hpp \
    boost/smart_ptr/*.hpp \
    boost/smart_ptr/detail/*.hpp \
    boost/utility/*.hpp \
    boost/utility/detail/*.hpp \
    qxrddexeladriver.h \
    qxrddexelaplugin.h

contains(QMAKE_HOST.arch,x86_64) {
    LIBS += -L$$PWD/dexelaLibs/windows_x64/ -lBusScanner
#} else {
#    LIBS += -L$$PWD/dexelaLibs/win32_x86/   -lBusScanner -lDexelaException -lDexelaDetector -lDexImage
}

OTHER_FILES += \
    dexelaLibs/windows_x64/*.lib \
    dexelaLibs/win32_x86/*.lib \
    dexela.json

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
