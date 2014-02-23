message(qceplib-tiff PWD = $${PWD})

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
