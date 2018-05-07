TEMPLATE = app
TARGET   = qxrdviewer
DESTDIR  = ../../

include("qxrdviewer.version.pri")
include("../../compiler.pri")
include("../../extras-app.pri")

DEFINES += QXRD_VERSION=\"$$VERSION\"

CONFIG += qceplib-qwt
CONFIG += qceplib-hdf5
CONFIG += qceplib-tiff

#CONFIG += qceplib-nexus

include(../../libraries/qceplib/qceplib/qceplib-qwt-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-base-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-mar345-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-cbf-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-tiff-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-levmar-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-szip-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-zlib-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-hdf5-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-bzip2-include.pri)
include(../../libraries/qceplib/qceplib/qceplib-specserver-include.pri)

INCLUDEPATH += $${PWD}/../../libraries/qxrdlib/
INCLUDEPATH += $${OUT_PWD}/../../libraries/qxrdlib/

SOURCES += qxrdviewer.cpp

macx:
else:unix:LIBS += -ltiff
else:win32 { 
    INCLUDEPATH += .
}

OTHER_FILES += qxrd-viewer.rc \
    qxrd-viewer.nsi

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqceplibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqceplib

INCLUDEPATH += $$PWD/../../libraries/qceplib
DEPENDPATH += $$PWD/../../libraries/qceplib

INCLUDEPATH += $$OUT_PWD/../../libraries/qceplib
DEPENDPATH  += $$OUT_PWD/../../libraries/qceplib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lqxrdlibd
else:unix: LIBS += -L$$OUT_PWD/../../ -lqxrdlib

INCLUDEPATH += $$PWD/../../libraries/qxrdlib
DEPENDPATH += $$PWD/../../libraries/qxrdlib

INCLUDEPATH += $$OUT_PWD/../../libraries/qxrdlib
DEPENDPATH  += $$OUT_PWD/../../libraries/qxrdlib
