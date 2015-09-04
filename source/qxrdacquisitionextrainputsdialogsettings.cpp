#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qcepplotsettings.h"

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings(QcepSettingsSaverWPtr saver, QcepObject *parent) :
  QcepObject("extraInputsDialog", parent)
{
  m_AcquisitionExtraInputsPlotSettings = QcepPlotSettingsPtr(new QcepPlotSettings("extraInputsPlot", saver, parent));
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

QcepPlotSettingsWPtr QxrdAcquisitionExtraInputsDialogSettings::plotSettings() const
{
  return m_AcquisitionExtraInputsPlotSettings;
}
