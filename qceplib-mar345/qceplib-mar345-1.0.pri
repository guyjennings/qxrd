message(qceplib-mar345 PWD = $${PWD} QCEPLIB_MAR345_VERSION = $${QCEPLIB_MAR345_VERSION})

include(qceplib-mar345-include-$${QCEPLIB_MAR345_VERSION}.pri)

SOURCES += \
  $${PWD}/qcepimagedataformatmar345.cpp

HEADERS += \
  $${PWD}/qcepimagedataformatmar345.h \
  $${PWD}/qcepimagedataformatmar345-ptr.h
