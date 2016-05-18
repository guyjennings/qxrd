#include "qxrddistortioncorrectiondialogsettings.h"
#include "qcepsettingssaver.h"

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings(QString name) :
  QcepSerializableObject(name)
{
}

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings() :
  QxrdDistortionCorrectionDialogSettings("distortionCorrectionDialogSettings")
{
}
