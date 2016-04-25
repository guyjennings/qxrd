#include "qcepdataimportparameters.h"
#include "qcepimagedata.h"
#include "qcepdatacolumnscan.h"

QcepDataImportParameters::QcepDataImportParameters(QcepObjectWPtr parent, QcepSettingsSaverWPtr saver, QString name) :
  QcepObject(name, parent),
  m_InputFormat(saver, this, "inputFormat", 0, "File format for data import"),
  m_LastImportPath(saver, this, "lastImportPath", "", "Last imported path"),
  m_SelectedFilter(saver, this, "selectedFilter", "", "Last import filter")
{
}

QString QcepDataImportParameters::fileFormatFilterString()
{
  return QcepImageDataBase::fileFormatTIFF() + ";;" +
      QcepDataObject::fileFormatHDF5() + ";;" +
      QcepDataObject::fileFormatNexus() + ";;" +
      QcepDataColumnScan::fileFormatSpec() + ";;" +
      QcepDataObject::fileFormatAny();
}

QString QcepDataImportParameters::lastDirectory()
{
  return get_LastImportPath();
}

void QcepDataImportParameters::setLastDirectory(QStringList files)
{
  set_LastImportPath(files.value(0));
}
