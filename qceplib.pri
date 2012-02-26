INCLUDEPATH += qceplib/
HEADERS += qceplib/spec_server.h \
    qceplib/qspecserver.h \
    qceplib/qcepmacros.h \
    qceplib/qcepproperty.h \
    qceplib/qcepproperty-ptr.h \
    qceplib/qcepimagedata.h \
    qceplib/qcepimagedata-ptr.h \
    qceplib/qcepimagedataformat.h \
    qceplib/qcepimagedataformat-ptr.h \
    qceplib/qcepimagedataformattiff.h \
    qceplib/qcepimagedataformatmar345.h \
    qceplib/qcepimagedataformatfactory.h \
    qceplib/qcepimagedataformatfactory-ptr.h \
    qceplib/qcepdebug.h
SOURCES += qceplib/qspecserver.cpp \
    qceplib/qcepproperty.cpp \
    qceplib/qcepimagedata.cpp \
    qceplib/qcepimagedataformat.cpp \
    qceplib/qcepimagedataformattiff.cpp \
    qceplib/qcepimagedataformatmar345.cpp \
    qceplib/qcepimagedataformatfactory.cpp \
    qceplib/qcepdebug.cpp
OTHER_FILES += source/testImage.js
