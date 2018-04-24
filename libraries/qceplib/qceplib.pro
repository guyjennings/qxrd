#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T10:48:55
#
#-------------------------------------------------

QT       += widgets qml network script concurrent

TEMPLATE = lib
DESTDIR  = ../../

qtHaveModule(datavisualization) {
    QT += datavisualization
    DEFINES += HAVE_DATAVIS
}

win32:CONFIG   += skip_target_version_ext

win32:CONFIG(debug, debug|release) {
  TARGET = qceplibd
} else {
  TARGET = qceplib
}

DEFINES += QCEPLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += qceplib-qwt
CONFIG += qceplib-hdf5
CONFIG += qceplib-tiff
CONFIG += qceplib-bzip2
CONFIG += qceplib-libzip

DEFINES += QWT_MAKEDLL
DEFINES += CBF_MAKEDLL
DEFINES += TIFF_MAKEDLL
DEFINES += MAR345_MAKEDLL
DEFINES += LEVMAR_MAKEDLL
DEFINES += SZIP_MAKEDLL
DEFINES += ZLIB_MAKEDLL
DEFINES += HDF5_MAKEDLL
DEFINES += SPECSERVER_MAKEDLL
DEFINES += BZIP2_MAKEDLL
DEFINES += LIBZIP_MAKEDLL

#DEFINES += H5_BUILT_AS_DYNAMIC_LIB

include(qceplib/qceplib-qwt.pri)
include(qceplib/qceplib-base.pri)
include(qceplib/qceplib-mar345.pri)
include(qceplib/qceplib-cbf.pri)
include(qceplib/qceplib-tiff.pri)
include(qceplib/qceplib-levmar.pri)
include(qceplib/qceplib-szip.pri)
include(qceplib/qceplib-zlib.pri)
include(qceplib/qceplib-hdf5.pri)
include(qceplib/qceplib-specserver.pri)
include(qceplib/qceplib-bzip2.pri)
#include(qceplib/qceplib-libzip.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}
