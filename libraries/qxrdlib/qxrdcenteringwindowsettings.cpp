#include "qxrdcenteringwindowsettings.h"
#include "qxrdcenteringwindow.h"
#include "qxrdfilebrowsersettings.h"

QxrdCenteringWindowSettings::QxrdCenteringWindowSettings(QString name)
  : QxrdMainWindowSettings(name),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name))
{

}

QxrdMainWindowPtr QxrdCenteringWindowSettings::newWindow()
{
  QxrdCenteringWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdCenteringWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdCenteringWindow(myself, "Centering", m_Application, m_Experiment));

  return m_Window;
}

void QxrdCenteringWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();
}

void QxrdCenteringWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdCenteringWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}
