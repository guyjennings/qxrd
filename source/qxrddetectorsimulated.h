#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorSimulated : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorSimulated(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);

  void initialize();

signals:

public slots:

};

#endif // QXRDDETECTORSIMULATED_H
