#include "qxrdacquisitionwindowsettings.h"
#include "qxrdacquisitionwindow.h"
#include "qcepfilebrowsersettings.h"
#include <QThread>

QxrdAcquisitionWindowSettings::QxrdAcquisitionWindowSettings(QString name)
  : inherited(name, "Acquisition Window"),
    m_FileBrowserSettings(NEWPTR(QcepFileBrowserSettings(name+"Browser")))
{
}

void QxrdAcquisitionWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_FileBrowserSettings -> initialize(sharedFromThis());
}

QcepMainWindowPtr QxrdAcquisitionWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  if (m_Window) {
    printMessage("Attempt to re-open window");
  } else {
    m_Window =
        QxrdMainWindowPtr(
          NEWPTR(QxrdAcquisitionWindow("Acquisition")));
  }

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

QcepFileBrowserSettingsPtr QxrdAcquisitionWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}
