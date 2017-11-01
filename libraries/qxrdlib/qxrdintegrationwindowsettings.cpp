#include "qxrdintegrationwindowsettings.h"
#include "qxrdintegrationwindow.h"

QxrdIntegrationWindowSettings::QxrdIntegrationWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdIntegrationWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdIntegrationWindow("Integration", m_Application, m_Experiment));

  return m_Window;
}
