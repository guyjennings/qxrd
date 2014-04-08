message(qceplib-zlib PWD = $${PWD} QCEPLIB_ZLIB_VERSION = $${QCEPLIB_ZLIB_VERSION})

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64
macx-*-32:QMAKE_TARGET.arch = x86

ZLIBBASE = $${PWD}/zlib-$${QCEPLIB_ZLIB_VERSION}/

macx {
  ZLIBCONF = $${PWD}/zlib-config/macx/
} else:unix {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    ZLIBCONF = $${PWD}/zlib-config/lin64/
  } else {
    message(32 bit build)
    ZLIBCONF = $${PWD}/zlib-config/lin32/
  }
} else:win32 {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    ZLIBCONF = $${PWD}/zlib-config/win64/
  } else {
    message(32 bit build)
    ZLIBCONF = $${PWD}/zlib-config/win32/
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

#macx {
#  CBFBASE = $${PWD}/CBFlib-0.9.3.3/src/
#  CBFINCL = $${PWD}/CBFlib-0.9.3.3/include/

#  DEFINES += CBF_NO_REGEX
#} else:win32 {
#  CBFBASE = $${PWD}/CBFlib-0.9.3.3/src/
#  CBFINCL = $${PWD}/CBFlib-0.9.3.3/include/

#  contains(QMAKE_TARGET.arch, x86_64) {
#    message(64 bit build)
#  } else {
#    message(32 bit build)
#  }

#  DEFINES += CBF_NO_REGEX
##  DEFINES += CBF_USE_ULP
#} else:unix {
#  INCLUDEPATH += /usr/include/cbf/
#  LIBS += -lcbf
#}

#macx|win32 {
#  INCLUDEPATH += $${CBFINCL}

#  HEADERS += \
#    $${CBFINCL}/cbf.h \
#    $${CBFINCL}/cbf_alloc.h \
#    $${CBFINCL}/cbf_ascii.h \
#    $${CBFINCL}/cbf_binary.h \
#    $${CBFINCL}/cbf_byte_offset.h \
#    $${CBFINCL}/cbf_canonical.h \
#    $${CBFINCL}/cbf_codes.h \
#    $${CBFINCL}/cbf_compress.h \
#    $${CBFINCL}/cbf_context.h \
#    $${CBFINCL}/cbf_copy.h \
#    $${CBFINCL}/cbf_file.h \
#    $${CBFINCL}/cbf_getopt.h \
#    $${CBFINCL}/cbf_lex.h \
#    $${CBFINCL}/cbf_nibble_offset.h \
#    $${CBFINCL}/cbf_packed.h \
#    $${CBFINCL}/cbf_predictor.h \
#    $${CBFINCL}/cbf_read_binary.h \
#    $${CBFINCL}/cbf_read_mime.h \
#    $${CBFINCL}/cbf_simple.h \
#    $${CBFINCL}/cbf_string.h \
#    $${CBFINCL}/cbf_stx.h \
#    $${CBFINCL}/cbf_tree.h \
#    $${CBFINCL}/cbf_ulp.h \
#    $${CBFINCL}/cbf_uncompressed.h \
#    $${CBFINCL}/cbf_write.h \
#    $${CBFINCL}/cbf_write_binary.h \
#    $${CBFINCL}/cbf_ws.h \
#    $${CBFINCL}/global.h \
#    $${CBFINCL}/md5.h

#  SOURCES += \
#    $${CBFBASE}/cbf.c \
#    $${CBFBASE}/cbf_alloc.c \
#    $${CBFBASE}/cbf_ascii.c \
#    $${CBFBASE}/cbf_binary.c \
#    $${CBFBASE}/cbf_byte_offset.c \
#    $${CBFBASE}/cbf_canonical.c \
#    $${CBFBASE}/cbf_codes.c \
#    $${CBFBASE}/cbf_compress.c \
#    $${CBFBASE}/cbf_context.c \
#    $${CBFBASE}/cbf_copy.c \
#    $${CBFBASE}/cbf_file.c \
#    $${CBFBASE}/cbf_getopt.c \
#    $${CBFBASE}/cbf_lex.c \
#    $${CBFBASE}/cbf_nibble_offset.c \
#    $${CBFBASE}/cbf_packed.c \
#    $${CBFBASE}/cbf_predictor.c \
#    $${CBFBASE}/cbf_read_binary.c \
#    $${CBFBASE}/cbf_read_mime.c \
#    $${CBFBASE}/cbf_simple.c \
#    $${CBFBASE}/cbf_string.c \
#    $${CBFBASE}/cbf_stx.c \
#    $${CBFBASE}/cbf_tree.c \
#    $${CBFBASE}/cbf_ulp.c \
#    $${CBFBASE}/cbf_uncompressed.c \
#    $${CBFBASE}/cbf_write.c \
#    $${CBFBASE}/cbf_write_binary.c \
#    $${CBFBASE}/cbf_ws.c \
#    $${CBFBASE}/md5c.c


#}

#macx {
#  HEADERS += \
#    $${CBFINCL}/cbf_hdf5.h \
#    $${CBFINCL}/cbf_hdf5_filter.h \
#    $${CBFINCL}/cbff.h

#  SOURCES += \
#    $${CBFBASE}/cbf_hdf5.c \
#    $${CBFBASE}/cbf_hdf5_filter.c \
#    $${CBFBASE}/cbff.c
#}

#SOURCES += \
#  $${PWD}/qcepimagedataformatcbf.cpp

#HEADERS += \
#  $${PWD}/qcepimagedataformatcbf.h

