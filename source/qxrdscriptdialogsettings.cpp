#include "qxrdscriptdialogsettings.h"
#include "qxrdsettingssaver.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QxrdSettingsSaverWPtr /*saver*/, QObject *parent) :
  QObject(parent)
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

