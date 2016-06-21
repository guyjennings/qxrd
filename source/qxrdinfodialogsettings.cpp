#include "qxrdinfodialogsettings.h"
#include "qcepmutexlocker.h"

QxrdInfoDialogSettings::QxrdInfoDialogSettings(QString name) :
  QcepSerializableObject(name)
{
}

QxrdInfoDialogSettingsPtr QxrdInfoDialogSettings::newInfoDialogSettings()
{
  QxrdInfoDialogSettingsPtr set(new QxrdInfoDialogSettings("infoDialogSettings"));

  return set;
}
