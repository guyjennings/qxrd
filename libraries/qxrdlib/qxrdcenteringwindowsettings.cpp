#include "qxrdcenteringwindowsettings.h"
#include "qxrdcenteringwindow.h"

QxrdCenteringWindowSettings::QxrdCenteringWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdCenteringWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdCenteringWindow("Centering", m_Application, m_Experiment));

  return m_Window;
}
