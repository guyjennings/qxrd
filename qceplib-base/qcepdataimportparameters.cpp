#include "qcepdataimportparameters.h"

QcepDataImportParameters::QcepDataImportParameters(QcepSettingsSaverWPtr saver, QString name) :
  QcepObject(name, NULL),
  m_InputFormat(saver, this, "inputFormat", 0, "File format for data import")
{

}
