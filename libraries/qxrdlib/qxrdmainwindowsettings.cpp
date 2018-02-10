#include "qxrdmainwindowsettings.h"
#include "qxrdmainwindow.h"
#include "qxrdappcommon.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qxrdprocessor.h"

QxrdMainWindowSettings::QxrdMainWindowSettings(QString name)
  : QcepMainWindowSettings(name)
{
}

void QxrdMainWindowSettings::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Application = QxrdAppCommon::findApplication(parent);
  m_Experiment  = QxrdExperiment::findExperiment(parent);
  m_Acquisition = QxrdAcquisition::findAcquisition(parent);
  m_Processor   = QxrdProcessor::findProcessor(parent);

  if (m_Application == NULL) {
    printMessage("QxrdMainWindowSettings::m_Application == NULL");
  }

  if (m_Experiment  == NULL) {
    printMessage("QxrdMainWindowSettings::m_Experiment == NULL");
  }

  if (m_Acquisition == NULL) {
    printMessage("QxrdMainWindowSettings::m_Acquisition == NULL");
  }

  if (m_Processor   == NULL) {
    printMessage("QxrdMainWindowSettings::m_Processor == NULL");
  }
}

QxrdMainWindowPtr QxrdMainWindowSettings::window()
{
  return m_Window;
}

void QxrdMainWindowSettings::openWindow()
{
  if (get_WindowOpen()) {
    newWindow();

    if (get_WindowRect().isValid()) {
      QRect geom = get_WindowRect().toAlignedRect();

      m_Window->setGeometry(geom);
    }

    m_Window->show();
    m_Window->raise();
    m_Window->activateWindow();
  }
}

void QxrdMainWindowSettings::closeWindow()
{
  set_WindowOpen(m_Window != NULL);

  if (m_Window) {
    QRect geom = m_Window->geometry();

    set_WindowRect(geom);

    m_Window = QxrdMainWindowPtr();
  }
}

void QxrdMainWindowSettings::readSettings(QSettings *settings)
{
  QcepMainWindowSettings::readSettings(settings);
}

void QxrdMainWindowSettings::writeSettings(QSettings *settings)
{
  set_WindowOpen(false);

  if (m_Window) {
    bool isVis = m_Window->isVisible();

    set_WindowOpen(isVis);

    if (isVis) {
      QRect bds = m_Window->geometry();

      set_WindowRect(bds);
    }
  }

  QcepMainWindowSettings::writeSettings(settings);
}
