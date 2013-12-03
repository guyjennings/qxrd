#include "qxrdsynchronizedacquisitiondialogsettings.h"

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent)
{
  m_SynchronizedAcquisitionPlotSettings = QxrdSynchronizedAcquisitionPlotSettingsPtr(
        new QxrdSynchronizedAcquisitionPlotSettings(saver, parent));
}

void QxrdSynchronizedAcquisitionDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_SynchronizedAcquisitionPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdSynchronizedAcquisitionDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  m_SynchronizedAcquisitionPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdSynchronizedAcquisitionPlotSettingsWPtr QxrdSynchronizedAcquisitionDialogSettings::synchronizedAcquisitionPlotSettings()
{
  return m_SynchronizedAcquisitionPlotSettings;
}
