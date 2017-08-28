message(qceplib-tiff PWD = $${PWD} QCEPLIB_TIFF_VERSION = $${QCEPLIB_TIFF_VERSION})

include(qceplib-tiff-include-$${QCEPLIB_TIFF_VERSION})

macx {
  include("$${PWD}/tiffconfig-$${QCEPLIB_TIFF_VERSION}/qt-libtiff-macx.pri")
} else:win32 {
  include("$${PWD}/tiffconfig-$${QCEPLIB_TIFF_VERSION}/qt-libtiff-win32.pri")
} else {
}

SOURCES += \
  $${PWD}/qcepimagedataformattiff.cpp

HEADERS += \
  $${PWD}/qcepimagedataformattiff.h
