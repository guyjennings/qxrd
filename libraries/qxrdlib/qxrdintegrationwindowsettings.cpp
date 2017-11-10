#include "qxrdintegrationwindowsettings.h"
#include "qxrdintegrationwindow.h"
#include "qxrdfilebrowsersettings.h"

QxrdIntegrationWindowSettings::QxrdIntegrationWindowSettings(QString name)
  : QxrdMainWindowSettings(name),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name))
{

}

QxrdMainWindowPtr QxrdIntegrationWindowSettings::newWindow()
{
  QxrdIntegrationWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdIntegrationWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdIntegrationWindow(myself, "Integration", m_Application, m_Experiment));

  return m_Window;
}

void QxrdIntegrationWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();
}

void QxrdIntegrationWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdIntegrationWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}
