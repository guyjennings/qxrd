#ifndef QCEPDATAEXPORTPARAMETERS_H
#define QCEPDATAEXPORTPARAMETERS_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

class QcepDataExportParameters : public QcepObject
{
  Q_OBJECT

public:
  QcepDataExportParameters(QcepSettingsSaverWPtr saver, QString name);

public:
  Q_PROPERTY(int outputFormat READ get_OutputFormat WRITE set_OutputFormat)
  QCEP_INTEGER_PROPERTY(OutputFormat)
};

#endif // QCEPDATAEXPORTPARAMETERS_H
