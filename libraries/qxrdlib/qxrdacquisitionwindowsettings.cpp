#include "qxrdacquisitionwindowsettings.h"
#include "qxrdacquisitionwindow.h"
#include "qxrdfilebrowsersettings.h"
#include <QThread>

QxrdAcquisitionWindowSettings::QxrdAcquisitionWindowSettings(QString name, QString desc)
  : QxrdMainWindowSettings(name, desc),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(desc))
{
}

QxrdMainWindowPtr QxrdAcquisitionWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

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
