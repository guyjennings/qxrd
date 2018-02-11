#include "qcepmainwindowsettings.h"

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
