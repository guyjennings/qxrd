#ifndef QXRDDETECTORPILATUS_H
#define QXRDDETECTORPILATUS_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorPilatus : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorPilatus(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);

signals:

public slots:

};

#endif // QXRDDETECTORPILATUS_H
