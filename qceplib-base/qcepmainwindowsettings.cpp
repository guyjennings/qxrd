#include "qcepmainwindowsettings.h"
#include "qcepmainwindow.h"

QcepMainWindowSettings::QcepMainWindowSettings(QString name, QString desc)
  : inherited(name),
    m_Description(this, "description", desc, "Window Description"),
    m_WindowOpen(this, "windowOpen", 0, "Is window open?"),
    m_WindowRect(this, "windowRect", QRectF(), "Window Geometry")
{

}

void QcepMainWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}


void QcepMainWindowSettings::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);
}

void QcepMainWindowSettings::writeSettings(QSettings *settings)
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

  inherited::writeSettings(settings);
}

QcepMainWindowPtr QcepMainWindowSettings::window()
{
  return m_Window;
}

void QcepMainWindowSettings::openWindow()
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

void QcepMainWindowSettings::closeWindow()
{
  set_WindowOpen(m_Window != NULL);

  if (m_Window) {
    QRect geom = m_Window->geometry();

    set_WindowRect(geom);

    m_Window = QcepMainWindowPtr();
  }
}

