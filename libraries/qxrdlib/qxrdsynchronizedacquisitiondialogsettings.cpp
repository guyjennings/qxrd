#include "qxrdsynchronizedacquisitiondialogsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings(QString name) :
  QcepObject(name)
{
  m_SynchronizedAcquisitionPlotSettings =
      QxrdSynchronizedAcquisitionPlotSettings::newSynchronizedAcquisitionPlotSettings();
}

QxrdSynchronizedAcquisitionDialogSettingsPtr QxrdSynchronizedAcquisitionDialogSettings::newSynchronizedAcquisitionDialogSettings()
{
  QxrdSynchronizedAcquisitionDialogSettingsPtr set(new QxrdSynchronizedAcquisitionDialogSettings("syncDialog"));

  return set;
}

void QxrdSynchronizedAcquisitionDialogSettings::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings);

  if (m_SynchronizedAcquisitionPlotSettings) {
    settings->beginGroup("plot");
    m_SynchronizedAcquisitionPlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdSynchronizedAcquisitionDialogSettings::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);

  if (m_SynchronizedAcquisitionPlotSettings) {
    settings->beginGroup("plot");
    m_SynchronizedAcquisitionPlotSettings->writeSettings(settings);
    settings->endGroup();
  }
}

QxrdSynchronizedAcquisitionPlotSettingsWPtr QxrdSynchronizedAcquisitionDialogSettings::synchronizedAcquisitionPlotSettings()
{
  return m_SynchronizedAcquisitionPlotSettings;
}
