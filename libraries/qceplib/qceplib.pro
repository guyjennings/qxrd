#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T10:48:55
#
#-------------------------------------------------

QT       += widgets qml network script concurrent datavisualization charts

win32:CONFIG(debug, debug|release) {
  TARGET = qceplibd
} else {
  TARGET = qceplib
}

TEMPLATE = lib
DESTDIR  = ../../

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

CONFIG += qceplib-qwt qceplib-hdf5 qceplib-tiff

DEFINES += QWT_MAKEDLL
DEFINES += CBF_MAKEDLL
DEFINES += TIFF_MAKEDLL
DEFINES += MAR345_MAKEDLL
DEFINES += LEVMAR_MAKEDLL
DEFINES += SZIP_MAKEDLL
DEFINES += ZLIB_MAKEDLL
DEFINES += HDF5_MAKEDLL
DEFINES += SPECSERVER_MAKEDLL

#DEFINES += H5_BUILT_AS_DYNAMIC_LIB

include(../../source/submodules/qceplib/qceplib-qwt.pri)
include(../../source/submodules/qceplib/qceplib-base.pri)
include(../../source/submodules/qceplib/qceplib-mar345.pri)
include(../../source/submodules/qceplib/qceplib-cbf.pri)
include(../../source/submodules/qceplib/qceplib-tiff.pri)
include(../../source/submodules/qceplib/qceplib-levmar.pri)
include(../../source/submodules/qceplib/qceplib-szip.pri)
include(../../source/submodules/qceplib/qceplib-zlib.pri)
include(../../source/submodules/qceplib/qceplib-hdf5.pri)
include(../../source/submodules/qceplib/qceplib-specserver.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}
