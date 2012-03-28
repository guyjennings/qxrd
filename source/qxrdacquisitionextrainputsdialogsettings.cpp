#include "qxrdacquisitionextrainputsdialogsettings.h"

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent)
{
  m_AcquisitionExtraInputsPlotSettings = QxrdPlotSettingsPtr(new QxrdPlotSettings(saver, parent));
}

void QxrdAcquisitionExtraInputsDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  m_AcquisitionExtraInputsPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdAcquisitionExtraInputsDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  m_AcquisitionExtraInputsPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdPlotSettingsWPtr QxrdAcquisitionExtraInputsDialogSettings::plotSettings() const
{
  return m_AcquisitionExtraInputsPlotSettings;
}
