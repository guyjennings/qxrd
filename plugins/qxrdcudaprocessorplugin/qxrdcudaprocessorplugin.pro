include("../../qxrd.version.pri")
include("../../qxrd.platform.pri")

TEMPLATE = lib
CONFIG += plugin

#include("cuda.pri")

DESTDIR = ../../app/plugins/

INCLUDEPATH += ../../source/

HEADERS += \
    qxrdcudaprocessorplugin.h

SOURCES += \
    qxrdcudaprocessorplugin.cpp

OTHER_FILES += qxrdcudakernel.cu

#CUDA_SOURCES += \
#  qxrdcudakernel.cu

