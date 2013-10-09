message(nexus PWD = $${PWD})

macx {
  NEXUSSRC  = $${PWD}/../nexus-4.3.1/src/
  NEXUSHDR  = $${PWD}/../nexus-4.3.1/include/
  NEXUSCONF = $${PWD}/../nexus-config/macx/
} else:win32 {
  NEXUSSRC  = $${PWD}/../nexus-4.3.1/src/
  NEXUSHDR  = $${PWD}/../nexus-4.3.1/include/
  NEXUSCONF = $${PWD}/../nexus-config/win32/
} else {
}

macx|win32 {
  INCLUDEPATH += $${NEXUSHDR} $${NEXUSCONF}

  DEFINES += HAVE_CONFIG_H=1
  DEFINES += HAVE_HDF5

  HEADERS += \
    $${NEXUSCONF}/nxconfig.h

  SOURCES += \
    $${NEXUSSRC}/napi5.c \
    $${NEXUSSRC}/napi.c \
    $${NEXUSSRC}/napiu.c \
    $${NEXUSSRC}/nxdataset.c \
    $${NEXUSSRC}/nxstack.c \
    $${NEXUSSRC}/stptok.c
}
