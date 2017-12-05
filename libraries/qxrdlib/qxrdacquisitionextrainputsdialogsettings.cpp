#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qcepplotsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdacquisitionextrainputsplotsettings.h"

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdAcquisitionExtraInputsDialogSettingsPtr QxrdAcquisitionExtraInputsDialogSettings::newAcquisitionExtraInputsDialogSettings()
{
  QxrdAcquisitionExtraInputsDialogSettingsPtr set(new QxrdAcquisitionExtraInputsDialogSettings("extraInputsDialog"));

  set -> addChildPtr(QxrdAcquisitionExtraInputsPlotSettings::newAcquisitionExtraInputsPlotSettings());

  return set;
}

void QxrdAcquisitionExtraInputsDialogSettings::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);

  if (checkPointer<QxrdAcquisitionExtraInputsPlotSettings>(child, m_AcquisitionExtraInputsPlotSettings)) {}
}

void QxrdAcquisitionExtraInputsDialogSettings::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings);

  if (m_AcquisitionExtraInputsPlotSettings) {
    settings->beginGroup("plot");
    m_AcquisitionExtraInputsPlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdAcquisitionExtraInputsDialogSettings::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);

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
