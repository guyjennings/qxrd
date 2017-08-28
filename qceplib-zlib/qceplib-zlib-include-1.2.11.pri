message(qceplib-zlib-include PWD = $${PWD} QCEPLIB_ZLIB_VERSION = $${QCEPLIB_ZLIB_VERSION})

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
