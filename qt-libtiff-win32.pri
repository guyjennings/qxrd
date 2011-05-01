win32 {
  INCLUDEPATH += tiff-3.8.2/libtiff/ tiffconfig/win32/

  HEADERS += \
        tiffconfig/win32/tif_config.h \
        tiffconfig/win32/tiffconf.h \
        tiff-3.8.2/libtiff/tiff.h \
        tiff-3.8.2/libtiff/tiffio.h \
        tiff-3.8.2/libtiff/tiffiop.h \
        tiff-3.8.2/libtiff/tiffvers.h \
        tiff-3.8.2/libtiff/uvcode.h \
        tiff-3.8.2/libtiff/tif_dir.h \
        tiff-3.8.2/libtiff/tif_fax3.h \
        tiff-3.8.2/libtiff/tif_predict.h

  SOURCES += \
        tiff-3.8.2/libtiff/tif_aux.c \
        tiff-3.8.2/libtiff/tif_close.c \
        tiff-3.8.2/libtiff/tif_codec.c \
        tiff-3.8.2/libtiff/tif_color.c \
        tiff-3.8.2/libtiff/tif_compress.c \
        tiff-3.8.2/libtiff/tif_dir.c \
        tiff-3.8.2/libtiff/tif_dirinfo.c \
        tiff-3.8.2/libtiff/tif_dirread.c \
        tiff-3.8.2/libtiff/tif_dirwrite.c \
        tiff-3.8.2/libtiff/tif_dumpmode.c \
        tiff-3.8.2/libtiff/tif_error.c \
        tiff-3.8.2/libtiff/tif_extension.c \
        tiff-3.8.2/libtiff/tif_fax3.c \
        tiff-3.8.2/libtiff/tif_fax3sm.c \
        tiff-3.8.2/libtiff/tif_flush.c \
        tiff-3.8.2/libtiff/tif_jpeg.c \
        tiff-3.8.2/libtiff/tif_luv.c \
        tiff-3.8.2/libtiff/tif_lzw.c \
        tiff-3.8.2/libtiff/tif_next.c \
        tiff-3.8.2/libtiff/tif_ojpeg.c \
        tiff-3.8.2/libtiff/tif_open.c \
        tiff-3.8.2/libtiff/tif_packbits.c \
        tiff-3.8.2/libtiff/tif_pixarlog.c \
        tiff-3.8.2/libtiff/tif_predict.c \
        tiff-3.8.2/libtiff/tif_print.c \
        tiff-3.8.2/libtiff/tif_read.c \
        tiff-3.8.2/libtiff/tif_strip.c \
        tiff-3.8.2/libtiff/tif_swab.c \
        tiff-3.8.2/libtiff/tif_thunder.c \
        tiff-3.8.2/libtiff/tif_tile.c \
        tiff-3.8.2/libtiff/tif_version.c \
        tiff-3.8.2/libtiff/tif_warning.c \
        tiff-3.8.2/libtiff/tif_win32.c \
        tiff-3.8.2/libtiff/tif_write.c \
        tiff-3.8.2/libtiff/tif_zip.c
}
