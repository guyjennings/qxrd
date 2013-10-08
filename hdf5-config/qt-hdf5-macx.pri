message(macx hdf5 PWD = $${PWD})

macx {
  HDF5BASE = $${PWD}/../hdf5-1.8.11/src/
  HDF5CONF = $${PWD}/../hdf5-config/macx/

  LIBS += -lz
}
