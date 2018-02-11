#include "qxrdcalibrantwindowsettings.h"
#include "qxrdcalibrantwindow.h"
#include <QThread>

QxrdCalibrantWindowSettings::QxrdCalibrantWindowSettings(QString name, QString desc)
  : QxrdMainWindowSettings(name, desc)
{
}

QxrdMainWindowPtr QxrdCalibrantWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdCalibrantWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdCalibrantWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdCalibrantWindow(myself, "Calibrant", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
