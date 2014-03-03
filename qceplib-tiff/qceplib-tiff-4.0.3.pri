message(qceplib-tiff PWD = $${PWD} QCEPLIB_TIFF_VERSION = $${QCEPLIB_TIFF_VERSION})

macx {
  include("$${PWD}/tiffconfig/qt-libtiff-macx.pri")
} else:win32 {
  include("$${PWD}/tiffconfig/qt-libtiff-win32.pri")
} else {
  LIBS += -ltiff
}

INCLUDEPATH += $${PWD}

SOURCES += \
  $${PWD}/qcepimagedataformattiff.cpp

HEADERS += \
  $${PWD}/qcepimagedataformattiff.h
