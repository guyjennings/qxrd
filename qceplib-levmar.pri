QCEPLIB_LEVMAR_VERSION = "2.6-1"

DEFINES += QCEPLIB_LEVMAR_VERSION=\"$${QCEPLIB_LEVMAR_VERSION}\"

include(qceplib-levmar/qceplib-levmar-$${QCEPLIB_LEVMAR_VERSION}.pri)
