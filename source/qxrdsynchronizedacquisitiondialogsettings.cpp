#include "qxrdsynchronizedacquisitiondialogsettings.h"
#include "qcepmutexlocker.h"

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings(QcepSettingsSaverWPtr saver, QcepObject *parent) :
  QObject(parent)
{
  m_SynchronizedAcquisitionPlotSettings = QxrdSynchronizedAcquisitionPlotSettingsPtr(
        new QxrdSynchronizedAcquisitionPlotSettings(saver, parent));
}

void QxrdSynchronizedAcquisitionDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings, section);

  m_SynchronizedAcquisitionPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSynchronizedAcquisitionDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings, section);

  m_SynchronizedAcquisitionPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdSynchronizedAcquisitionPlotSettingsWPtr QxrdSynchronizedAcquisitionDialogSettings::synchronizedAcquisitionPlotSettings()
{
  return m_SynchronizedAcquisitionPlotSettings;
}
