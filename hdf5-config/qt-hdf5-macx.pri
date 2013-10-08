message(hdf5 PWD = $${PWD})

macx {
  HDF5BASE = $${PWD}/../hdf5-1.8.11/src/
  HDF5CONF = $${PWD}/../hdf5-config/macx/

  INCLUDEPATH += $${HDF5BASE} $${HDF5CONF}

  HEADERS +=

  SOURCES +=

}
