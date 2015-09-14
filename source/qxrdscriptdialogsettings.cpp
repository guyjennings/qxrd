#include "qxrdscriptdialogsettings.h"
#include "qcepsettingssaver.h"
#include "qcepmutexlocker.h"

QxrdScriptDialogSettings::QxrdScriptDialogSettings(QcepSettingsSaverWPtr /*saver*/, QcepObject *parent) :
  QcepObject("scriptDialogSettings", parent)
{
}

void QxrdScriptDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings, section);
}

void QxrdScriptDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings, section);
}

