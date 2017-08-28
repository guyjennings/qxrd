message(qceplib-zlib PWD = $${PWD} QCEPLIB_ZLIB_VERSION = $${QCEPLIB_ZLIB_VERSION})

include(qceplib-zlib-include-$${QCEPLIB_ZLIB_VERSION}.pri)

HEADERS += \
  $${ZLIBCONF}/zconf.h \
  $${ZLIBBASE}/crc32.h \
  $${ZLIBBASE}/deflate.h \
  $${ZLIBBASE}/gzguts.h \
  $${ZLIBBASE}/inffast.h \
  $${ZLIBBASE}/inffixed.h \
  $${ZLIBBASE}/inflate.h \
  $${ZLIBBASE}/inftrees.h \
  $${ZLIBBASE}/trees.h \
  $${ZLIBBASE}/zlib.h \
  $${ZLIBBASE}/zutil.h

SOURCES += \
  $${ZLIBBASE}/adler32.c \
  $${ZLIBBASE}/compress.c \
  $${ZLIBBASE}/crc32.c \
  $${ZLIBBASE}/deflate.c \
  $${ZLIBBASE}/gzclose.c \
  $${ZLIBBASE}/gzlib.c \
  $${ZLIBBASE}/gzread.c \
  $${ZLIBBASE}/gzwrite.c \
  $${ZLIBBASE}/infback.c \
  $${ZLIBBASE}/inffast.c \
  $${ZLIBBASE}/inflate.c \
  $${ZLIBBASE}/inftrees.c \
  $${ZLIBBASE}/trees.c \
  $${ZLIBBASE}/uncompr.c \
  $${ZLIBBASE}/zutil.c
