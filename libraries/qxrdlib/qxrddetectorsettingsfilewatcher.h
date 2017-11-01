#ifndef QXRDDETECTORSETTINGSFILEWATCHER_H
#define QXRDDETECTORSETTINGSFILEWATCHER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

class QXRD_EXPORT QxrdDetectorSettingsFileWatcher : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  explicit QxrdDetectorSettingsFileWatcher(QxrdApplicationWPtr app, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:
  
public slots:
  
};

#endif // QXRDDETECTORSETTINGSFILEWATCHER_H
