TEMPLATE = lib
CONFIG += plugin

target.path = ../../app/plugins/

INSTALLS += target

INCLUDEPATH += ../../source/

HEADERS += \
    qxrdpilatusplugin.h

SOURCES += \
    qxrdpilatusplugin.cpp
