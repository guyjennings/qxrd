message(qceplib-szip PWD = $${PWD} QCEPLIB_SZIP_VERSION = $${QCEPLIB_SZIP_VERSION})

include(qceplib-szip-include-$${QCEPLIB_SZIP_VERSION}.pri)

HEADERS += \
  $${SZIPCONF}/SZconfig.h \
  $${SZIPBASE}/rice.h \
  $${SZIPBASE}/ricehdf.h \
  $${SZIPBASE}/szip_adpt.h \
  $${SZIPBASE}/szlib.h

SOURCES += \
  $${SZIPBASE}/encoding.c \
  $${SZIPBASE}/rice.c \
  $${SZIPBASE}/sz_api.c
