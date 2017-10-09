QCEPLIB_VERSION = 0.1.20

win32 {
  DEFINES += NOMINMAX
}

qceplib-base {
  DEFINES += HAVE_BASE
}

qceplib-qwt {
  DEFINES += HAVE_QWT
}

qceplib-tiff {
  DEFINES += HAVE_TIFF
}

qceplib-cbf {
  DEFINES += HAVE_CBF
}

qceplib-hdf5 {
  DEFINES += HAVE_HDF5
}

qceplib-levmar {
  DEFINES += HAVE_LEVMAR
}

qceplib-nexus {
  DEFINES += HAVE_NEXUS
}

qceplib-mar345 {
  DEFINES += HAVE_MAR345
}

qceplib-specserver {
  DEFINES += HAVE_SPECSERVER
}

qceplib-szip {
  DEFINES += HAVE_SZIP
}

qceplib-zlib {
  DEFINES += HAVE_ZLIB
}
