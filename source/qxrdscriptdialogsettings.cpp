#include "qxrdscriptdialogsettings.h"
#include "qcepsettingssaver.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QcepSettingsSaverWPtr /*saver*/, QcepObject *parent) :
  QcepObject("scriptDialogSettings", parent)
{
}

void QxrdScriptDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);
}

void QxrdScriptDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);
}

