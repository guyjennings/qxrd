message(qceplib-tiff-include PWD = $${PWD} QCEPLIB_TIFF_VERSION = $${QCEPLIB_TIFF_VERSION})

macx {
  include("$${PWD}/tiffconfig-$${QCEPLIB_TIFF_VERSION}/qt-libtiff-macx-include.pri")
} else:win32 {
  include("$${PWD}/tiffconfig-$${QCEPLIB_TIFF_VERSION}/qt-libtiff-win32-include.pri")
} else {
  LIBS += -ltiff
}

INCLUDEPATH += $${PWD}
