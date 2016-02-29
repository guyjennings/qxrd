#ifndef QXRDPOLARNORMALIZATION_H
#define QXRDPOLARNORMALIZATION_H

#include "qcepmacros.h"

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepimagedata-ptr.h"

class QxrdPolarNormalization : public QcepObject
{
  Q_OBJECT

public:
  QxrdPolarNormalization(QcepSettingsSaverWPtr saver,
                         QxrdExperimentWPtr    exp);

  void execute();

private:
  void executeCol(QcepDoubleImageDataPtr dst, QcepDoubleImageDataPtr img, int col);

public:
  Q_PROPERTY(QString source READ get_Source WRITE set_Source)
  QCEP_STRING_PROPERTY(Source)

  Q_PROPERTY(QString destination READ get_Destination WRITE set_Destination)
  QCEP_STRING_PROPERTY(Destination)

private:
  mutable QMutex        m_Mutex;
  QcepSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
};

#endif // QXRDPOLARNORMALIZATION_H
