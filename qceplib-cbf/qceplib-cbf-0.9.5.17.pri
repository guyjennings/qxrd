message(qceplib-cbf PWD = $${PWD} QCEPLIB_CBF_VERSION = $${QCEPLIB_CBF_VERSION})

include(qceplib-cbf-include-$${QCEPLIB_CBF_VERSION}.pri)

macx|win32 {
  HEADERS += \
    $${CBFINCL}/cbf.h \
    $${CBFINCL}/cbf_airy_disk.h \
    $${CBFINCL}/cbf_alloc.h \
    $${CBFINCL}/cbf_ascii.h \
    $${CBFINCL}/cbf_binary.h \
    $${CBFINCL}/cbf_byte_offset.h \
    $${CBFINCL}/cbf_canonical.h \
    $${CBFINCL}/cbf_codes.h \
    $${CBFINCL}/cbf_compress.h \
    $${CBFINCL}/cbf_context.h \
    $${CBFINCL}/cbf_copy.h \
    $${CBFINCL}/cbf_file.h \
    $${CBFINCL}/cbf_getopt.h \
    $${CBFINCL}/cbf_lex.h \
    $${CBFINCL}/cbf_minicbf_header.h \
    $${CBFINCL}/cbf_nibble_offset.h \
    $${CBFINCL}/cbf_packed.h \
    $${CBFINCL}/cbf_predictor.h \
    $${CBFINCL}/cbf_read_binary.h \
    $${CBFINCL}/cbf_read_mime.h \
    $${CBFINCL}/cbf_simple.h \
    $${CBFINCL}/cbf_string.h \
    $${CBFINCL}/cbf_stx.h \
    $${CBFINCL}/cbf_tree.h \
    $${CBFINCL}/cbf_ulp.h \
    $${CBFINCL}/cbf_uncompressed.h \
    $${CBFINCL}/cbf_write.h \
    $${CBFINCL}/cbf_write_binary.h \
    $${CBFINCL}/cbf_ws.h \
    $${CBFINCL}/global.h \
    $${CBFINCL}/img.h \
    $${CBFINCL}/md5.h

  SOURCES += \
    $${CBFBASE}/cbf.c \
    $${CBFBASE}/cbf_airy_disk.c \
    $${CBFBASE}/cbf_alloc.c \
    $${CBFBASE}/cbf_ascii.c \
    $${CBFBASE}/cbf_binary.c \
    $${CBFBASE}/cbf_byte_offset.c \
    $${CBFBASE}/cbf_canonical.c \
    $${CBFBASE}/cbf_codes.c \
    $${CBFBASE}/cbf_compress.c \
    $${CBFBASE}/cbf_context.c \
    $${CBFBASE}/cbf_copy.c \
    $${CBFBASE}/cbf_file.c \
    $${CBFBASE}/cbf_getopt.c \
    $${CBFBASE}/cbf_lex.c \
    $${CBFBASE}/cbf_minicbf_header.c \
    $${CBFBASE}/cbf_nibble_offset.c \
    $${CBFBASE}/cbf_packed.c \
    $${CBFBASE}/cbf_predictor.c \
    $${CBFBASE}/cbf_read_binary.c \
    $${CBFBASE}/cbf_read_mime.c \
    $${CBFBASE}/cbf_simple.c \
    $${CBFBASE}/cbf_string.c \
    $${CBFBASE}/cbf_stx.c \
    $${CBFBASE}/cbf_tree.c \
    $${CBFBASE}/cbf_ulp.c \
    $${CBFBASE}/cbf_uncompressed.c \
    $${CBFBASE}/cbf_write.c \
    $${CBFBASE}/cbf_write_binary.c \
    $${CBFBASE}/cbf_ws.c \
    $${CBFBASE}/fgetln.c \
    $${CBFBASE}/img.c \
    $${CBFBASE}/md5c.c


}

macx {
  HEADERS += \
    $${CBFINCL}/cbf_hdf5.h \
    $${CBFINCL}/cbf_hdf5_filter.h \
    $${CBFINCL}/cbff.h

  SOURCES += \
    $${CBFBASE}/cbf_hdf5.c \
    $${CBFBASE}/cbf_hdf5_filter.c \
    $${CBFBASE}/cbff.c
}

SOURCES += \
  $${PWD}/qcepimagedataformatcbf.cpp

HEADERS += \
  $${PWD}/qcepimagedataformatcbf.h

