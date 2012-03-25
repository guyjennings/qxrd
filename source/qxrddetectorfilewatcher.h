#ifndef QXRDDETECTORFILEWATCHER_H
#define QXRDDETECTORFILEWATCHER_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorFileWatcher : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorFileWatcher(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
  
  void initialize();

signals:
  
public slots:
  
};

#endif // QXRDDETECTORFILEWATCHER_H