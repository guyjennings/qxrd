#include "qxrdinfowindowsettings.h"
#include "qxrdinfowindow.h"

QxrdInfoWindowSettings::QxrdInfoWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdInfoWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdInfoWindow("Info", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
