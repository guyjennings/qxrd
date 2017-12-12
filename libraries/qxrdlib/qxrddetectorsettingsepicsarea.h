#ifndef QXRDDETECTORSETTINGSEPICSAREA_H
#define QXRDDETECTORSETTINGSEPICSAREA_H

#include "qxrdlib_global.h"
#include "qxrddetectorsettings.h"

//TODO: rename to QxrdAreaDetectorSettings

class QXRD_EXPORT QxrdDetectorSettingsEpicsArea : public QxrdDetectorSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDetectorSettingsEpicsArea(QString name);
  virtual ~QxrdDetectorSettingsEpicsArea();

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

  void configureDetector();
};

Q_DECLARE_METATYPE(QxrdDetectorSettingsEpicsArea*)

#endif // QXRDDETECTORSETTINGSEPICSAREA_H
