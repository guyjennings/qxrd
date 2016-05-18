#include "qxrdsynchronizedacquisitiondialogsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings() :
  QxrdSynchronizedAcquisitionDialogSettings("syncDialog")
{
  addChildPtr(QxrdSynchronizedAcquisitionPlotSettingsPtr(
                new QxrdSynchronizedAcquisitionPlotSettings()));
}

void QxrdSynchronizedAcquisitionDialogSettings::addChildPtr(QcepSerializableObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

  if (checkPointer<QxrdSynchronizedAcquisitionPlotSettings>(child, m_SynchronizedAcquisitionPlotSettings)) {}
}

void QxrdSynchronizedAcquisitionDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  m_SynchronizedAcquisitionPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSynchronizedAcquisitionDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  m_SynchronizedAcquisitionPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdSynchronizedAcquisitionPlotSettingsWPtr QxrdSynchronizedAcquisitionDialogSettings::synchronizedAcquisitionPlotSettings()
{
  return m_SynchronizedAcquisitionPlotSettings;
}
