macx {
  TIFFBASE = $${QCEPLIB}/tiff-4.0.3/libtiff/
  TIFFCONF = $${QCEPLIB}/tiffconfig/macx/

  INCLUDEPATH += $${TIFFBASE} $${TIFFCONF}

  HEADERS += \
        $${TIFFCONF}/tif_config.h \
        $${TIFFCONF}/tiffconf.h \
        $${TIFFBASE}/tiff.h \
        $${TIFFBASE}/tiffio.h \
        $${TIFFBASE}/tiffiop.h \
        $${TIFFBASE}/tiffvers.h \
        $${TIFFBASE}/uvcode.h \
        $${TIFFBASE}/tif_dir.h \
        $${TIFFBASE}/tif_fax3.h \
        $${TIFFBASE}/tif_predict.h

  SOURCES += \
        $${TIFFBASE}/tif_aux.c \
        $${TIFFBASE}/tif_close.c \
        $${TIFFBASE}/tif_codec.c \
        $${TIFFBASE}/tif_color.c \
        $${TIFFBASE}/tif_compress.c \
        $${TIFFBASE}/tif_dir.c \
        $${TIFFBASE}/tif_dirinfo.c \
        $${TIFFBASE}/tif_dirread.c \
        $${TIFFBASE}/tif_dirwrite.c \
        $${TIFFBASE}/tif_dumpmode.c \
        $${TIFFBASE}/tif_error.c \
        $${TIFFBASE}/tif_extension.c \
        $${TIFFBASE}/tif_fax3.c \
        $${TIFFBASE}/tif_fax3sm.c \
        $${TIFFBASE}/tif_flush.c \
        $${TIFFBASE}/tif_jpeg.c \
        $${TIFFBASE}/tif_luv.c \
        $${TIFFBASE}/tif_lzw.c \
        $${TIFFBASE}/tif_next.c \
        $${TIFFBASE}/tif_ojpeg.c \
        $${TIFFBASE}/tif_open.c \
        $${TIFFBASE}/tif_packbits.c \
#        $${TIFFBASE}/tif_pixarlog.c \
        $${TIFFBASE}/tif_predict.c \
        $${TIFFBASE}/tif_print.c \
        $${TIFFBASE}/tif_read.c \
        $${TIFFBASE}/tif_strip.c \
        $${TIFFBASE}/tif_swab.c \
        $${TIFFBASE}/tif_thunder.c \
        $${TIFFBASE}/tif_tile.c \
        $${TIFFBASE}/tif_version.c \
        $${TIFFBASE}/tif_warning.c \
        $${TIFFBASE}/tif_unix.c \
        $${TIFFBASE}/tif_write.c \
#        $${TIFFBASE}/tif_zip.c
}
