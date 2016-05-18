#include "qxrdscriptdialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QString name) :
  QcepSerializableObject(name)
{
}

QxrdScriptDialogSettings::QxrdScriptDialogSettings() :
  QxrdScriptDialogSettings("scriptDialogSettings")
{
}
