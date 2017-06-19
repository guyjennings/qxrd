#ifndef QCEPDATAPROCESSORBASE_H
#define QCEPDATAPROCESSORBASE_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepimagedata-ptr.h"

class QcepDataProcessorBase : public QcepObject
{
  Q_OBJECT

public:
  QcepDataProcessorBase(QString name);

public slots:
  virtual QcepDataObjectPtr integrate(QcepDoubleImageDataPtr img) = 0;
  virtual QcepDataObjectPtr polarTransform(QcepDoubleImageDataPtr img) = 0;
  virtual QcepDataObjectPtr polarIntegrate(QcepDoubleImageDataPtr img) = 0;

  virtual bool integrateParameters() = 0;
  virtual bool polarTransformParameters() = 0;
  virtual bool polarIntegrateParameters() = 0;
};

#endif // QCEPDATAPROCESSORBASE_H
