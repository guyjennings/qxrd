#include "qxrdcalibrantwindowsettings.h"
#include "qxrdcalibrantwindow.h"

QxrdCalibrantWindowSettings::QxrdCalibrantWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{
}

QxrdMainWindowPtr QxrdCalibrantWindowSettings::newWindow()
{
  QxrdCalibrantWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdCalibrantWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdCalibrantWindow(myself, "Calibrant", m_Application, m_Experiment));

  return m_Window;
}
