#include "qcepdataexportparameters.h"
#include "qcepimagedata.h"
#include "qcepdatacolumnscan.h"

QcepDataExportParameters::QcepDataExportParameters(QcepSettingsSaverWPtr saver, QString  name) :
  QcepObject(name, NULL),
  m_OutputFormat(saver, this, "outputFormat", 0, "File format for data export"),
  m_FileName(saver, this, "fileName", "", "Last exported file name"),
  m_SelectedFilter(saver, this, "selectedFilter", "", "Last export filter")
{
}

QString QcepDataExportParameters::fileFormatFilterString()
{
  return QcepDataObject::fileFormatTIFF() + ";;" +
      QcepDataObject::fileFormatHDF5() + ";;" +
      QcepDataObject::fileFormatNexus() + ";;" +
      QcepDataObject::fileFormatSpec() + ";;" +
      QcepDataObject::fileFormatTabDelimited() + ";;" +
      QcepDataObject::fileFormatAny();
}
