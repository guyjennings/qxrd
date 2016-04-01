#ifndef QCEPDATAIMPORTPARAMETERS_H
#define QCEPDATAIMPORTPARAMETERS_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"

class QcepDataImportParameters : public QcepObject
{
  Q_OBJECT

public:
  QcepDataImportParameters(QcepSettingsSaverWPtr saver, QString name);

public:
  Q_PROPERTY(int inputFormat READ get_InputFormat WRITE set_InputFormat)
  QCEP_INTEGER_PROPERTY(InputFormat)
};

#endif // QCEPDATAIMPORTPARAMETERS_H
