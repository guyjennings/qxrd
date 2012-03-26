#ifndef QXRDDETECTORFILEWATCHER_H
#define QXRDDETECTORFILEWATCHER_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorFileWatcher : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorFileWatcher(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
  int detectorType() const;

signals:
  
public slots:
  
};

#endif // QXRDDETECTORFILEWATCHER_H
