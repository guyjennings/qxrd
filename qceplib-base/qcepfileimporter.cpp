#include "qcepfileimporter.h"
#include "qcepfileimporterhdf.h"
#include "qcepfileimportertext.h"
#include "qcepfileimportertiff.h"
#include "qcepfileimportermultiple.h"
#include "qcepdatasetmodel.h"

#ifdef HAVE_HDF5
#include "hdf5.h"
#endif

#ifdef HAVE_TIFF
#include <tiff.h>
#include <tiffio.h>
#endif

QcepFileImporter::QcepFileImporter(QcepDatasetModelPtr model,
                                   QModelIndexList &indexes,
                                   QString path)
  : QcepObject("importer"),
    m_Model(model),
    m_Indexes(indexes),
    m_Path(path)
{

}

static bool isHDF(QString path)
{
#ifdef HAVE_HDF5
  bool res = false;

  if (H5Fis_hdf5(qPrintable(path))) {
    res = true;
  }

  return res;
#else
  return false;
#endif
}

static bool isTIFF(QString path)
{
#ifdef HAVE_TIFF
  bool res = false;

#ifdef TIFF_VERSION
  TIFFHeader header;
#else
  TIFFHeaderCommon header;
#endif
  FILE *file = fopen(qPrintable(path), "r");

  if (file) {
    if (fread(&header, sizeof(header), 1, file) == 1) {
      switch (header.tiff_magic) {
      case TIFF_BIGENDIAN:
      case TIFF_LITTLEENDIAN:
      case MDI_LITTLEENDIAN:
      case MDI_BIGENDIAN:
        res = true;
      }
    }

    fclose(file);
  }

  return res;
#else
  return false;
#endif
}

static bool isText(QString /*path*/)
{
  return true;
}

QcepFileImporterPtr
QcepFileImporter::importFile(QcepDatasetModelPtr model,
                             QModelIndexList &indexes,
                             QString path)
{
  QcepFileImporterPtr res;

  if (isHDF(path)) {
    res = QcepFileImporterPtr(
          new QcepFileImporterHDF(model, indexes, path));
  } else if (isTIFF(path)) {
    res = QcepFileImporterPtr(
          new QcepFileImporterTIFF(model, indexes, path));
  } else if (isText(path)) {
    res = QcepFileImporterPtr(
          new QcepFileImporterText(model, indexes, path));
  }

  return res;
}

QcepFileImporterPtr
QcepFileImporter::importFiles(QcepDatasetModelPtr model,
                              QModelIndexList &indexes,
                              QStringList paths)
{
  QcepFileImporterMultiple *res = NULL;

  if (paths.count() == 1) {
    return importFile(model, indexes, paths.value(0));
  } else if (paths.count() >= 1) {
    res = new QcepFileImporterMultiple(model, indexes);

    if (res) {
      foreach(QString path, paths) {
        if (isHDF(path)) {
          res-> append(QcepFileImporterPtr(new QcepFileImporterHDF(model, indexes, path)));
        } else if (isTIFF(path)) {
          res -> append(QcepFileImporterPtr(new QcepFileImporterTIFF(model, indexes, path)));
        } else if (isText(path)) {
          res -> append(QcepFileImporterPtr(new QcepFileImporterText(model, indexes, path)));
        }
      }
    }
  }

  return QcepFileImporterPtr(res);
}

void QcepFileImporter::exec()
{
}
