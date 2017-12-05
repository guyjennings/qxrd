#include "qxrdmainwindowsettings.h"
#include "qxrdmainwindow.h"

QxrdMainWindowSettings::QxrdMainWindowSettings(QString name)
  : QcepMainWindowSettings(name),
    m_WindowOpen(this, "windowOpen", 0, "Is window open?"),
    m_WindowRect(this, "windowRect", QRectF(), "Window Geometry")
{
}

void QxrdMainWindowSettings::initialize(QxrdApplicationWPtr app, QxrdExperimentWPtr expt)
{
  m_Application = app;
  m_Experiment  = expt;
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
