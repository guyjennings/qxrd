TEMPLATE = lib
CONFIG += plugin

target.path = ../../app/plugins/

INSTALLS += target

INCLUDEPATH += ../../source/

HEADERS += \
    qxrdperkinelmerplugin.h

SOURCES += \
    qxrdperkinelmerplugin.cpp
