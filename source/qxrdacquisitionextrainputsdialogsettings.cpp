#include "qxrdacquisitionextrainputsdialogsettings.h"

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings(QcepSettingsSaverWPtr saver, QObject *parent) :
  QcepObject("extraInputsDialog", parent)
{
  m_AcquisitionExtraInputsPlotSettings = QxrdPlotSettingsPtr(new QxrdPlotSettings("extraInputsPlot", saver, parent));
}

void QxrdAcquisitionExtraInputsDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);

  m_AcquisitionExtraInputsPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdAcquisitionExtraInputsDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);

  m_AcquisitionExtraInputsPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdPlotSettingsWPtr QxrdAcquisitionExtraInputsDialogSettings::plotSettings() const
{
  return m_AcquisitionExtraInputsPlotSettings;
}
