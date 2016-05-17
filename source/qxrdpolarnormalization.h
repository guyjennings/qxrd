#ifndef QXRDPOLARNORMALIZATION_H
#define QXRDPOLARNORMALIZATION_H

#include "qcepmacros.h"

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qxrdpolarnormalization-ptr.h"

class QxrdPolarNormalization : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdPolarNormalization(QString name);
  static QxrdPolarNormalizationPtr newPolarNormalization();
  QcepDataObjectPtr transform(QcepDoubleImageDataPtr img);

  enum {
    IntegratedOutput,
    FittedData,
    FittedDeltas
  };

private:
  void executeCol(QcepDataColumnScanPtr integ, QcepDoubleImageDataPtr dst, QcepDoubleImageDataPtr img, int col);

public:
  Q_PROPERTY(int outputType READ get_OutputType WRITE set_OutputType)
  QCEP_INTEGER_PROPERTY(OutputType)

  Q_PROPERTY(bool selfNormalize READ get_SelfNormalize WRITE set_SelfNormalize)
  QCEP_BOOLEAN_PROPERTY(SelfNormalize)

  Q_PROPERTY(double selfNormalizeMin READ get_SelfNormalizeMin WRITE set_SelfNormalizeMin)
  QCEP_DOUBLE_PROPERTY(SelfNormalizeMin)

  Q_PROPERTY(double selfNormalizeMax READ get_SelfNormalizeMax WRITE set_SelfNormalizeMax)
  QCEP_DOUBLE_PROPERTY(SelfNormalizeMax)
};

Q_DECLARE_METATYPE(QxrdPolarNormalization*)

#endif // QXRDPOLARNORMALIZATION_H
