#include "qcepdataexportparameters.h"
#include "qcepimagedata.h"
#include "qcepdatacolumnscan.h"
#include "qcepsettingssaver.h"

QcepDataExportParameters::QcepDataExportParameters(QcepObjectWPtr parent, QcepSettingsSaverWPtr saver, QString  name) :
  QcepObject(name, parent),
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
