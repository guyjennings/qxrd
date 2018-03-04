#include "qxrdacquisitionwindowsettings.h"
#include "qxrdacquisitionwindow.h"
#include "qxrdfilebrowsersettings.h"
#include <QThread>

QxrdAcquisitionWindowSettings::QxrdAcquisitionWindowSettings(QString name, QString desc)
  : inherited(name, desc),
    m_FileBrowserSettings(new QxrdFileBrowserSettings(desc))
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
          new QxrdAcquisitionWindow("Acquisition"),
          &QObject::deleteLater);
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

QxrdFileBrowserSettingsPtr QxrdAcquisitionWindowSettings::fileBrowserSettings()
{
  return m_FileBrowserSettings;
}
