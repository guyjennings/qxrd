#include "qxrdinfodialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdInfoDialogSettings::QxrdInfoDialogSettings(QcepSettingsSaverWPtr /*saver*/, QcepObject *parent) :
  QcepObject("infoDialogSettings", parent)
{
}

void QxrdInfoDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings, section);
}

void QxrdInfoDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings, section);
}

