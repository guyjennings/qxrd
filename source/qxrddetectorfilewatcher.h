#ifndef QXRDDETECTORFILEWATCHER_H
#define QXRDDETECTORFILEWATCHER_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorFileWatcher : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorFileWatcher(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, QcepObject *parent);

signals:
  
public slots:
  
};

#endif // QXRDDETECTORFILEWATCHER_H
