#ifndef QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
#define QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qxrddistortioncorrectiondialogsettings-ptr.h"

class QXRD_EXPORT QxrdDistortionCorrectionDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrectionDialogSettings(QString name);
  static QxrdDistortionCorrectionDialogSettingsPtr newDistortionCorrectionDialogSettings();
};

Q_DECLARE_METATYPE(QxrdDistortionCorrectionDialogSettings*)

#endif // QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
