#include "qcepdataexportparameters.h"
#include "qcepimagedata.h"
#include "qcepdatacolumnscan.h"

QcepDataExportParameters::QcepDataExportParameters(QString  name) :
  inherited(name),
  m_OutputFormat  (this, "outputFormat", 0, "File format for data export"),
  m_FileName      (this, "fileName", "", "Last exported file name"),
  m_SelectedFilter(this, "selectedFilter", "", "Last export filter")
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
