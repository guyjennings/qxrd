#include "qxrdinfowindowsettings.h"
#include "qxrdinfowindow.h"
#include <QThread>

QxrdInfoWindowSettings::QxrdInfoWindowSettings(QString name)
  : inherited(name, "Info Window")
{

}

void QxrdInfoWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdInfoWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        NEWPTR(QxrdInfoWindow("Info")));

  return m_Window;
}
