#include "qxrdscriptdialogsettings.h"
#include "qxrdsettingssaver.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QxrdSettingsSaverWPtr /*saver*/, QObject *parent) :
  QObject(parent)
{
}

void QxrdScriptDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdScriptDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

