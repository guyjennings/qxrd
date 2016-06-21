#ifndef QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
#define QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qxrddistortioncorrectiondialogsettings-ptr.h"

class QxrdDistortionCorrectionDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrectionDialogSettings(QString name);
  static QxrdDistortionCorrectionDialogSettingsPtr newDistortionCorrectionDialogSettings();
};

Q_DECLARE_METATYPE(QxrdDistortionCorrectionDialogSettings*)

#endif // QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
