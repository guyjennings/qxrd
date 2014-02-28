message(qceplib-specserver PWD = $${PWD} QCEPLIB_SPECSERVER_VERSION = $${QCEPLIB_SPECSERVER_VERSION})

QT += network script

INCLUDEPATH += $${PWD}

SOURCES += \
  $${PWD}/qspecserver.cpp

HEADERS += \
  $${PWD}/qspecserver.h \
  $${PWD}/spec_server.h
