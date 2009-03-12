win32 {
  INCLUDEPATH += ../qxrd/tiff-3.8.2/libtiff/ ../qxrd/

  HEADERS += \
        ../qxrd/tif_config.h \
        ../qxrd/tiffconf.h \
        ../qxrd/tiff-3.8.2/libtiff/tiff.h \
        ../qxrd/tiff-3.8.2/libtiff/tiffio.h \
        ../qxrd/tiff-3.8.2/libtiff/tiffiop.h \
        ../qxrd/tiff-3.8.2/libtiff/tiffvers.h \
        ../qxrd/tiff-3.8.2/libtiff/uvcode.h \
        ../qxrd/tiff-3.8.2/libtiff/tif_dir.h \
        ../qxrd/tiff-3.8.2/libtiff/tif_fax3.h \
        ../qxrd/tiff-3.8.2/libtiff/tif_predict.h

  SOURCES += \
        ../qxrd/tiff-3.8.2/libtiff/tif_aux.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_close.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_codec.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_color.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_compress.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_dir.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_dirinfo.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_dirread.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_dirwrite.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_dumpmode.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_error.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_extension.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_fax3.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_fax3sm.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_flush.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_jpeg.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_luv.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_lzw.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_next.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_ojpeg.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_open.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_packbits.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_pixarlog.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_predict.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_print.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_read.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_strip.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_swab.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_thunder.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_tile.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_version.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_warning.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_win32.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_write.c \
        ../qxrd/tiff-3.8.2/libtiff/tif_zip.c
}
