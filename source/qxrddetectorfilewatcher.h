#ifndef QXRDDETECTORFILEWATCHER_H
#define QXRDDETECTORFILEWATCHER_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorFileWatcher : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorFileWatcher(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:
  
public slots:
  
};

#endif // QXRDDETECTORFILEWATCHER_H
