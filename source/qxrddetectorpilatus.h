#ifndef QXRDDETECTORPILATUS_H
#define QXRDDETECTORPILATUS_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorPilatus : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorPilatus(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
  int detectorType() const;

signals:

public slots:

};

#endif // QXRDDETECTORPILATUS_H
