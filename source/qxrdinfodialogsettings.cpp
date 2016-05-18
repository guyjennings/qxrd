#include "qxrdinfodialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdInfoDialogSettings::QxrdInfoDialogSettings(QString name) :
  QcepSerializableObject(name)
{
}

QxrdInfoDialogSettings::QxrdInfoDialogSettings() :
  QxrdInfoDialogSettings("infoDialogSettings")
{
}
