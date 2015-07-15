include(qceplib-version.pri)

DEFINES += HAVE_BASE QCEPLIB_VERSION=\"$${QCEPLIB_VERSION}\"

include(qceplib-base/qceplib-base-$${QCEPLIB_VERSION}.pri)
