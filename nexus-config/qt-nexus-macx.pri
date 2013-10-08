message(nexus PWD = $${PWD})

macx {
  NEXUSSRC  = $${PWD}/../nexus-4.3.1/src/
  NEXUSHDR  = $${PWD}/../nexus-4.3.1/include/
  NEXUSCONF = $${PWD}/../nexus-config/macx/

  INCLUDEPATH += $${NEXUSHDR} $${NEXUSCONF}

  HEADERS +=

  SOURCES +=

}
