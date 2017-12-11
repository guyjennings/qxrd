#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTimer>

//TODO: rename to QxrdSimulatedSettings

class QXRD_EXPORT QxrdDetectorSettingsSimulated : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDetectorSettingsSimulated(QString name);
  virtual ~QxrdDetectorSettingsSimulated();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

};

Q_DECLARE_METATYPE(QxrdDetectorSettingsSimulated*)

#endif // QXRDDETECTORSIMULATED_H
