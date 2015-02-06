#include "qxrdsynchronizedacquisitiondialogsettings.h"

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent)
{
  m_SynchronizedAcquisitionPlotSettings = QxrdSynchronizedAcquisitionPlotSettingsPtr(
        new QxrdSynchronizedAcquisitionPlotSettings(saver, parent));
}

void QxrdSynchronizedAcquisitionDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);

  m_SynchronizedAcquisitionPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSynchronizedAcquisitionDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);

  m_SynchronizedAcquisitionPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdSynchronizedAcquisitionPlotSettingsWPtr QxrdSynchronizedAcquisitionDialogSettings::synchronizedAcquisitionPlotSettings()
{
  return m_SynchronizedAcquisitionPlotSettings;
}
