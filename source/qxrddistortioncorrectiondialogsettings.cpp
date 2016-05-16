#include "qxrddistortioncorrectiondialogsettings.h"
#include "qcepsettingssaver.h"

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings() :
  QxrdDistortionCorrectionDialogSettings("distortionCorrectionDialogSettings")
{
}
