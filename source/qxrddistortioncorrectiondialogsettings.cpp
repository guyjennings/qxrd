#include "qxrddistortioncorrectiondialogsettings.h"
#include "qcepsettingssaver.h"

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings(QcepSettingsSaverWPtr /*saver*/, QcepObjectWPtr parent) :
  QcepObject("distortionCorrectionDialogSettings", parent)
{
}

void QxrdDistortionCorrectionDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);
}

void QxrdDistortionCorrectionDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);
}
