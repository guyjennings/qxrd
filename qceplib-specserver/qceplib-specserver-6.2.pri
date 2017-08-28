message(qceplib-specserver PWD = $${PWD} QCEPLIB_SPECSERVER_VERSION = $${QCEPLIB_SPECSERVER_VERSION})

include(qceplib-specserver-include-$${QCEPLIB_SPECSERVER_VERSION}.pri)

SOURCES += \
  $${PWD}/qspecserver.cpp \
  $${PWD}/qspecremote.cpp

HEADERS += \
  $${PWD}/qspecserver.h \
  $${PWD}/qspecremote.h \
  $${PWD}/spec_server.h
