message(qceplib-hdf5-include PWD = $${PWD} QCEPLIB_HDF5_VERSION = $${QCEPLIB_HDF5_VERSION})

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64
macx-*-32:QMAKE_TARGET.arch = x86

HDF5BASE    = $${PWD}/hdf5-$${QCEPLIB_HDF5_VERSION}/src/
HDF5CPPBASE = $${PWD}/hdf5-$${QCEPLIB_HDF5_VERSION}/c++/src/

macx {
  HDF5CONF = $${PWD}/hdf5-config-$${QCEPLIB_HDF5_VERSION}/macx/
} else:win32 {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    HDF5CONF = $${PWD}/hdf5-config-$${QCEPLIB_HDF5_VERSION}/win64/
  } else {
    message(32 bit build)
    HDF5CONF = $${PWD}/hdf5-config-$${QCEPLIB_HDF5_VERSION}/win32/
  }
} else:unix {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    HDF5CONF = $${PWD}/hdf5-config-$${QCEPLIB_HDF5_VERSION}/lin64/
  } else {
    message(32 bit build)
    HDF5CONF = $${PWD}/hdf5-config-$${QCEPLIB_HDF5_VERSION}/lin32/
  }
  DEFINES += _LARGEFILE_SOURCE
  DEFINES += _LARGEFILE64_SOURCE
  LIBS += -ldl -lz
}

INCLUDEPATH += $${HDF5BASE} $${HDF5CONF} $${HDF5CPPBASE}
