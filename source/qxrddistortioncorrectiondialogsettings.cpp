#include "qxrddistortioncorrectiondialogsettings.h"
#include "qxrdsettingssaver.h"

QxrdDistortionCorrectionDialogSettings::QxrdDistortionCorrectionDialogSettings(QxrdSettingsSaverWPtr /*saver*/, QObject *parent) :
  QObject(parent)
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
