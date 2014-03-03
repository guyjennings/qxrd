message(qceplib-nexus PWD = $${PWD} QCEPLIB_NEXUS_VERSION = $${QCEPLIB_NEXUS_VERSION})

macx {
  NEXUSSRC  = $${PWD}/nexus-4.3.1/src/
  NEXUSHDR  = $${PWD}/nexus-4.3.1/include/
  NEXUSCONF = $${PWD}/nexus-config/macx/
} else:win32 {
  NEXUSSRC  = $${PWD}/nexus-4.3.1/src/
  NEXUSHDR  = $${PWD}/nexus-4.3.1/include/
  NEXUSCONF = $${PWD}/nexus-4.3.1/Windows_extra/include/
} else {
  NEXUSSRC  = $${PWD}/nexus-4.3.1/src/
  NEXUSHDR  = $${PWD}/nexus-4.3.1/include/
  NEXUSCONF = $${PWD}/nexus-config/linux/
}

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
