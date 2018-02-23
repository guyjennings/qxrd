#include "qxrdinfowindowsettings.h"
#include "qxrdinfowindow.h"
#include <QThread>

QxrdInfoWindowSettings::QxrdInfoWindowSettings(QString name, QString desc)
  : inherited(name, desc)
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
        new QxrdInfoWindow("Info", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
