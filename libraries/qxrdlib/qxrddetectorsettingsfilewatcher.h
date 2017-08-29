#ifndef QXRDDETECTORSETTINGSFILEWATCHER_H
#define QXRDDETECTORSETTINGSFILEWATCHER_H

#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

class QxrdDetectorSettingsFileWatcher : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  explicit QxrdDetectorSettingsFileWatcher(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:
  
public slots:
  
};

#endif // QXRDDETECTORSETTINGSFILEWATCHER_H
