#ifndef QXRDPOLARNORMALIZATION_H
#define QXRDPOLARNORMALIZATION_H

#include "qcepmacros.h"

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepintegrateddata-ptr.h"

class QxrdPolarNormalization : public QcepObject
{
  Q_OBJECT

public:
  QxrdPolarNormalization(QcepSettingsSaverWPtr saver,
                         QxrdExperimentWPtr    exp);

  void execute();

private:
  void executeCol(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dst, QcepDoubleImageDataPtr img, int col);

public:
  Q_PROPERTY(QString source READ get_Source WRITE set_Source)
  QCEP_STRING_PROPERTY(Source)

  Q_PROPERTY(QString destination READ get_Destination WRITE set_Destination)
  QCEP_STRING_PROPERTY(Destination)

  Q_PROPERTY(QString integrated READ get_Integrated WRITE set_Integrated)
  QCEP_STRING_PROPERTY(Integrated)

  Q_PROPERTY(bool selfNormalize READ get_SelfNormalize WRITE set_SelfNormalize)
  QCEP_BOOLEAN_PROPERTY(SelfNormalize)

  Q_PROPERTY(double selfNormalizeMin READ get_SelfNormalizeMin WRITE set_SelfNormalizeMin)
  QCEP_DOUBLE_PROPERTY(SelfNormalizeMin)

  Q_PROPERTY(double selfNormalizeMax READ get_SelfNormalizeMax WRITE set_SelfNormalizeMax)
  QCEP_DOUBLE_PROPERTY(SelfNormalizeMax)

private:
  mutable QMutex        m_Mutex;
  QcepSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
};

#endif // QXRDPOLARNORMALIZATION_H
