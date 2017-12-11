#ifndef QXRDDETECTORSETTINGSFILEWATCHER_H
#define QXRDDETECTORSETTINGSFILEWATCHER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

//TODO: rename to QxrdFileWatcherSettings

class QXRD_EXPORT QxrdDetectorSettingsFileWatcher : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDetectorSettingsFileWatcher(QString name);

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:
  
public slots:
  
};

Q_DECLARE_METATYPE(QxrdDetectorSettingsFileWatcher*)

#endif // QXRDDETECTORSETTINGSFILEWATCHER_H
