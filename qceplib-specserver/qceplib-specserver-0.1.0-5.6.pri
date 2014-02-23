message(qceplib-specserver PWD = $${PWD})

QT += network script

INCLUDEPATH += $${PWD}

SOURCES += \
  $${PWD}/qspecserver.cpp

HEADERS += \
  $${PWD}/qspecserver.h \
  $${PWD}/spec_server.h
