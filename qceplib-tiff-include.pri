QCEPLIB_TIFF_VERSION = "4.0.8"

DEFINES += QCEPLIB_TIFF_VERSION=\"$${QCEPLIB_TIFF_VERSION}\"

include(qceplib-tiff/qceplib-tiff-include-$${QCEPLIB_TIFF_VERSION}.pri)