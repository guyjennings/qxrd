#include "qxrdinfodialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdInfoDialogSettings::QxrdInfoDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdInfoDialogSettings::QxrdInfoDialogSettings() :
  QxrdInfoDialogSettings("infoDialogSettings")
{
}
