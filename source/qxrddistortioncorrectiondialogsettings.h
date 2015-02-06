#ifndef QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
#define QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdDistortionCorrectionDialogSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdDistortionCorrectionDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

typedef QSharedPointer<QxrdDistortionCorrectionDialogSettings> QxrdDistortionCorrectionDialogSettingsPtr;
typedef QWeakPointer<QxrdDistortionCorrectionDialogSettings>   QxrdDistortionCorrectionDialogSettingsWPtr;

#endif // QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
