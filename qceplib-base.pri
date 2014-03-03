include(qceplib-version.pri)

DEFINES += QCEPLIB_VERSION=\"$${QCEPLIB_VERSION}\"

include(qceplib-base/qceplib-base-$${QCEPLIB_VERSION}.pri)
