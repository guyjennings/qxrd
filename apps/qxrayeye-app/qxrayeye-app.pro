TEMPLATE = app
TARGET   = qxrayeye
DESTDIR  = ../../

include("qxrayeye.version.pri")
include("../../qxrd.platform.pri")
include("../../compiler.pri")
include("../../extras-app.pri")

DEFINES += QSE_VERSION=\"$$VERSION\"

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
INCLUDEPATH += $${OUT_PWD}/../../libraries/qceplib/ui/
INCLUDEPATH += $${OUT_PWD}/../../libraries/qxrdlib/ui/

SOURCES += \
  qxrayeye-app.cpp

