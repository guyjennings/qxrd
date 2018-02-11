#include "qxrdsynchronizedacquisitiondialogsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

QxrdSynchronizedAcquisitionDialogSettings::QxrdSynchronizedAcquisitionDialogSettings(QString name) :
  inherited(name)
{
  m_SynchronizedAcquisitionPlotSettings =
      QxrdSynchronizedAcquisitionPlotSettings::newSynchronizedAcquisitionPlotSettings();
}

QxrdSynchronizedAcquisitionDialogSettingsPtr QxrdSynchronizedAcquisitionDialogSettings::newSynchronizedAcquisitionDialogSettings()
{
  QxrdSynchronizedAcquisitionDialogSettingsPtr set(new QxrdSynchronizedAcquisitionDialogSettings("syncDialog"));

  return set;
}

void QxrdSynchronizedAcquisitionDialogSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  if (m_SynchronizedAcquisitionPlotSettings) {
    m_SynchronizedAcquisitionPlotSettings -> initialize(sharedFromThis());
  }
}

void QxrdSynchronizedAcquisitionDialogSettings::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  if (m_SynchronizedAcquisitionPlotSettings) {
    settings->beginGroup("plot");
    m_SynchronizedAcquisitionPlotSettings->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdSynchronizedAcquisitionDialogSettings::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

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
