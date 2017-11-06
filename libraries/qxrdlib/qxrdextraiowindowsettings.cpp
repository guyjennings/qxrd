#include "qxrdextraiowindowsettings.h"
#include "qxrdextraiowindow.h"

QxrdExtraIOWindowSettings::QxrdExtraIOWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdExtraIOWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdExtraIOWindow("Extra I/O", m_Application, m_Experiment));

  return m_Window;
}
