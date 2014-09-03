#include "qxrdinfodialogsettings.h"
#include "qxrdsettingssaver.h"

QxrdInfoDialogSettings::QxrdInfoDialogSettings(QxrdSettingsSaverWPtr /*saver*/, QObject *parent) :
  QObject(parent)
{
}

void QxrdInfoDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);
}

void QxrdInfoDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);
}

