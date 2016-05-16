#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qcepplotsettings.h"
#include "qcepmutexlocker.h"
#include "qxrdacquisitionextrainputsplotsettings.h"

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings(QString name) :
  QcepObject(name)
{
}

QxrdAcquisitionExtraInputsDialogSettings::QxrdAcquisitionExtraInputsDialogSettings() :
  QxrdAcquisitionExtraInputsDialogSettings("extraInputsDialog")
{
  addChildPtr(QxrdAcquisitionExtraInputsPlotSettingsPtr(
                new QxrdAcquisitionExtraInputsPlotSettings()));
}

void QxrdAcquisitionExtraInputsDialogSettings::addChildPtr(QcepObjectPtr child)
{
  QcepObject::addChildPtr(child);

  if (checkPointer<QxrdAcquisitionExtraInputsPlotSettings>(child, m_AcquisitionExtraInputsPlotSettings)) {}
}

void QxrdAcquisitionExtraInputsDialogSettings::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings, section);

  m_AcquisitionExtraInputsPlotSettings->readSettings(settings, section+"/plot");
}

void QxrdAcquisitionExtraInputsDialogSettings::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings, section);

  m_AcquisitionExtraInputsPlotSettings->writeSettings(settings, section+"/plot");
}

QxrdAcquisitionExtraInputsPlotSettingsWPtr QxrdAcquisitionExtraInputsDialogSettings::plotSettings() const
{
  return m_AcquisitionExtraInputsPlotSettings;
}
