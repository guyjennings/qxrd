#include "qxrdinfowindowsettings.h"
#include "qxrdinfowindow.h"
#include <QThread>

QxrdInfoWindowSettings::QxrdInfoWindowSettings(QString name, QString desc)
  : QxrdMainWindowSettings(name, desc)
{

}

QxrdMainWindowPtr QxrdInfoWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdInfoWindow("Info", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
