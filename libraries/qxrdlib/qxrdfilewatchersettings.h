#ifndef QXRDFILEWATCHERSETTINGS_H
#define QXRDFILEWATCHERSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

//TODO: rename to QxrdFileWatcherSettings

class QXRD_EXPORT QxrdFileWatcherSettings : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdFileWatcherSettings(QString name);

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

  void configureDetector();
signals:
  
public slots:
  
};

Q_DECLARE_METATYPE(QxrdFileWatcherSettings*)

#endif // QXRDFILEWATCHERSETTINGS_H
