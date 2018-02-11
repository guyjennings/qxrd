#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qcepplotsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdacquisitionextrainputsplotsettings.h"

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings(QString name) :
  inherited(name)
{
  m_AcquisitionExtraInputsPlotSettings =
      QxrdAcquisitionExtraInputsPlotSettings::newAcquisitionExtraInputsPlotSettings();
}

QxrdAcquisitionExtraInputsDialogSettingsPtr QxrdAcquisitionExtraInputsDialogSettings::newAcquisitionExtraInputsDialogSettings()
{
  QxrdAcquisitionExtraInputsDialogSettingsPtr set(new QxrdAcquisitionExtraInputsDialogSettings("extraInputsDialog"));

  return set;
}

void QxrdAcquisitionExtraInputsDialogSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  if (m_AcquisitionExtraInputsPlotSettings) {
    m_AcquisitionExtraInputsPlotSettings -> initialize(sharedFromThis());
  }
}
void QxrdAcquisitionExtraInputsDialogSettings::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  if (m_AcquisitionExtraInputsPlotSettings) {
    settings->beginGroup("plot");
    m_AcquisitionExtraInputsPlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdAcquisitionExtraInputsDialogSettings::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (m_AcquisitionExtraInputsPlotSettings) {
    settings->beginGroup("plot");
    m_AcquisitionExtraInputsPlotSettings->writeSettings(settings);
    settings->endGroup();
  }
}

QxrdAcquisitionExtraInputsPlotSettingsWPtr QxrdAcquisitionExtraInputsDialogSettings::plotSettings() const
{
  return m_AcquisitionExtraInputsPlotSettings;
}
