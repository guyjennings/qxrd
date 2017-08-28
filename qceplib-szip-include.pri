QCEPLIB_SZIP_VERSION = "2.1.1"

DEFINES += QCEPLIB_SZIP_VERSION=\"$${QCEPLIB_SZIP_VERSION}\"

include(qceplib-szip/qceplib-szip-include-$${QCEPLIB_SZIP_VERSION}.pri)
