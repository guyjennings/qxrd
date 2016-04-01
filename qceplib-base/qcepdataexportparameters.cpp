#include "qcepdataexportparameters.h"

QcepDataExportParameters::QcepDataExportParameters(QcepSettingsSaverWPtr saver, QString  name) :
  QcepObject(name, NULL),
  m_OutputFormat(saver, this, "outputFormat", 0, "File format for data export")
{
}
