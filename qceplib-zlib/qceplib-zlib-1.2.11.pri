message(qceplib-zlib PWD = $${PWD} QCEPLIB_ZLIB_VERSION = $${QCEPLIB_ZLIB_VERSION})

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64
macx-*-32:QMAKE_TARGET.arch = x86

ZLIBBASE = $${PWD}/zlib-$${QCEPLIB_ZLIB_VERSION}/

macx {
  ZLIBCONF = $${PWD}/zlib-config-$${QCEPLIB_ZLIB_VERSION}/macx/
} else:unix {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    ZLIBCONF = $${PWD}/zlib-config-$${QCEPLIB_ZLIB_VERSION}/lin64/
  } else {
    message(32 bit build)
    ZLIBCONF = $${PWD}/zlib-config-$${QCEPLIB_ZLIB_VERSION}/lin32/
  }
} else:win32 {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    ZLIBCONF = $${PWD}/zlib-config-$${QCEPLIB_ZLIB_VERSION}/win64/
  } else {
    message(32 bit build)
    ZLIBCONF = $${PWD}/zlib-config-$${QCEPLIB_ZLIB_VERSION}/win32/
  }
}

INCLUDEPATH += $${ZLIBCONF} $${ZLIBBASE}

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
