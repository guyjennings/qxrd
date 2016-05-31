#include "qxrddistortioncorrectiondialogsettings.h"
#include "qcepsettingssaver.h"

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings(QString name) :
  QcepSerializableObject(name)
{
}

QxrdDistortionCorrectionDialogSettingsPtr QxrdDistortionCorrectionDialogSettings::newDistortionCorrectionDialogSettings()
{
  QxrdDistortionCorrectionDialogSettingsPtr set(new QxrdDistortionCorrectionDialogSettings("distortionCorrectionDialogSettings"));

  return set;
}
