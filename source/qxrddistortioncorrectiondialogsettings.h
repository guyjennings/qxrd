#ifndef QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
#define QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdDistortionCorrectionDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdDistortionCorrectionDialogSettings(QcepSettingsSaverWPtr saver, QcepObjectWPtr parent);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

typedef QSharedPointer<QxrdDistortionCorrectionDialogSettings> QxrdDistortionCorrectionDialogSettingsPtr;
typedef QWeakPointer<QxrdDistortionCorrectionDialogSettings>   QxrdDistortionCorrectionDialogSettingsWPtr;

#endif // QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
