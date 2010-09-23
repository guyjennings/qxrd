TEMPLATE = lib
CONFIG += plugin

include("cuda.pri")

target.path = ../../app/plugins/

INSTALLS += target

INCLUDEPATH += ../../source/

HEADERS += \
    qxrdcudaprocessorplugin.h

SOURCES += \
    qxrdcudaprocessorplugin.cpp

OTHER_FILES += qxrdcudakernel.cu

CUDA_SOURCES += \
  qxrdcudakernel.cu

