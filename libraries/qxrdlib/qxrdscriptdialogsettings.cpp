#include "qxrdscriptdialogsettings.h"
#include "qcepmutexlocker.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdScriptDialogSettingsPtr QxrdScriptDialogSettings::newScriptDialogSettings()
{
  QxrdScriptDialogSettingsPtr set(new QxrdScriptDialogSettings("scriptDialogSettings"));

  return set;
}
