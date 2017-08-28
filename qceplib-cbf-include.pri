QCEPLIB_CBF_VERSION = "0.9.5.17"

DEFINES += QCEPLIB_CBF_VERSION=\"$${QCEPLIB_CBF_VERSION}\"

include(qceplib-cbf/qceplib-cbf-include-$${QCEPLIB_CBF_VERSION}.pri)
