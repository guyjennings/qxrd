message(libtiff PWD = $${PWD})

macx {
  TIFFBASE = $${PWD}/../tiff-$${QCEPLIB_TIFF_VERSION}/libtiff/
  TIFFCONF = $${PWD}/../tiffconfig-$${QCEPLIB_TIFF_VERSION}/macx/

  INCLUDEPATH += $${TIFFBASE} $${TIFFCONF}
}
