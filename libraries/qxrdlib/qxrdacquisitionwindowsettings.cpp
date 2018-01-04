#include "qxrdacquisitionwindowsettings.h"
#include "qxrdacquisitionwindow.h"
#include "qxrdfilebrowsersettings.h"

QxrdAcquisitionWindowSettings::QxrdAcquisitionWindowSettings(QString name)
  : QxrdMainWindowSettings(name),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name))
{
}

QxrdMainWindowPtr QxrdAcquisitionWindowSettings::newWindow()
{
  QxrdAcquisitionWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdAcquisitionWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdAcquisitionWindow(myself, "Acquisition", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}

void QxrdAcquisitionWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();
}

void QxrdAcquisitionWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdAcquisitionWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}
