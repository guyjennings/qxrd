#include "qcepfileimporter.h"
#include "qcepfileimporterhdf.h"
#include "qcepfileimportertext.h"
#include "qcepfileimportertiff.h"

QcepFileImporter::QcepFileImporter(QcepDatasetModelPtr model,
                                   QModelIndexList &indexes,
                                   QString path)
  : QcepObject("importer", NULL),
    m_Model(model),
    m_Indexes(indexes),
    m_Path(path)
{

}

static bool isHDF(QString path)
{
  return false;
}

static bool isTIFF(QString path)
{
  return false;
}

static bool isText(QString path)
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
    res -> exec();
  } else if (isTIFF(path)) {
    res = QcepFileImporterPtr(
          new QcepFileImporterTIFF(model, indexes, path));
    res -> exec();
  } else if (isText(path)) {
    res = QcepFileImporterPtr(
          new QcepFileImporterText(model, indexes, path));
    res -> exec();
  }

  return res;
}

QcepFileImporterPtr
QcepFileImporter::importFiles(QcepDatasetModelPtr model,
                              QModelIndexList &indexes,
                              QStringList paths)
{
  foreach(QString path, paths) {
    if (isHDF(path)) {
      QcepFileImporterHDF importer(model, indexes, path);
      importer.exec();
    } else if (isTIFF(path)) {
      QcepFileImporterTIFF importer(model, indexes, path);
      importer.exec();
    } else if (isText(path)) {
      QcepFileImporterText importer(model, indexes, path);
      importer.exec();
    }
  }

  return QcepFileImporterPtr(
        new QcepFileImporter(model, indexes, ""));
}

void QcepFileImporter::start()
{
}

void QcepFileImporter::exec()
{
}
