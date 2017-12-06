#ifndef QXRDPERKINELMERDRIVER_H
#define QXRDPERKINELMERDRIVER_H

#include "qxrddetectordriver.h"

class QxrdPerkinElmerDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdPerkinElmerDriver(QString name,
                        QxrdDetectorSettingsWPtr det,
                        QxrdExperimentWPtr expt,
                        QxrdAcquisitionWPtr acq);
};

#endif // QXRDPERKINELMERDRIVER_H
