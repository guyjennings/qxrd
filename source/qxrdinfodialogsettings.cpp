#include "qxrdinfodialogsettings.h"

QxrdInfoDialogSettings::QxrdInfoDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_Saver(saver)
{
}

void QxrdInfoDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdInfoDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

