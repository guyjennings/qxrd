#include "qxrdhelpwindowsettings.h"
#include "qxrdhelpwindow.h"

QxrdHelpWindowSettings::QxrdHelpWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdHelpWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdHelpWindow("Help", m_Application, m_Experiment));

  return m_Window;
}
