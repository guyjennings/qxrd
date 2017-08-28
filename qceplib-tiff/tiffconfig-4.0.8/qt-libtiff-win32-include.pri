win32 {
  TIFFBASE = $${PWD}/../tiff-$${QCEPLIB_TIFF_VERSION}/libtiff/
  TIFFCONF = $${PWD}/../tiffconfig-$${QCEPLIB_TIFF_VERSION}/win32/

  INCLUDEPATH += $${TIFFBASE} $${TIFFCONF}
}
