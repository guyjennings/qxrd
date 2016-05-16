#include "qxrdscriptdialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdScriptDialogSettings::QxrdScriptDialogSettings() :
  QxrdScriptDialogSettings("scriptDialogSettings")
{
}
