#include "qxrdcalibrantwindowsettings.h"
#include "qxrdcalibrantwindow.h"

QxrdCalibrantWindowSettings::QxrdCalibrantWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{
}

QxrdMainWindowPtr QxrdCalibrantWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdCalibrantWindow("Calibrant", m_Application, m_Experiment));

  return m_Window;
}
