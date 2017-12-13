#ifndef QXRDSIMULATEDSETTINGS_H
#define QXRDSIMULATEDSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTimer>

//TODO: rename to QxrdSimulatedSettings

class QXRD_EXPORT QxrdSimulatedSettings : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSimulatedSettings(QString name);
  virtual ~QxrdSimulatedSettings();

  void configureDetector();
};

Q_DECLARE_METATYPE(QxrdSimulatedSettings*)

#endif // QXRDSIMULATEDSETTINGS_H
