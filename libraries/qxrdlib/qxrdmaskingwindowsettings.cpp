#include "qxrdmaskingwindowsettings.h"
#include "qxrdmaskingwindow.h"
#include "qxrdfilebrowsersettings.h"

QxrdMaskingWindowSettings::QxrdMaskingWindowSettings(QString name)
  : QxrdMainWindowSettings(name),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(name))
{

}

QxrdMainWindowPtr QxrdMaskingWindowSettings::newWindow()
{
  QxrdMaskingWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdMaskingWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdMaskingWindow(myself, "Masking", m_Application, m_Experiment));

  return m_Window;
}

void QxrdMaskingWindowSettings::readSettings(QSettings *set)
{
  QxrdMainWindowSettings::readSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->readSettings(set);
  set->endGroup();
}

void QxrdMaskingWindowSettings::writeSettings(QSettings *set)
{
  QxrdMainWindowSettings::writeSettings(set);

  set->beginGroup("browser");
  m_FileBrowserSettings->writeSettings(set);
  set->endGroup();
}

QxrdFileBrowserSettingsPtr QxrdMaskingWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}
