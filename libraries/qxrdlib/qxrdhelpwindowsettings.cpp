#include "qxrdhelpwindowsettings.h"
#include "qxrdhelpwindow.h"
#include <QThread>

QxrdHelpWindowSettings::QxrdHelpWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdHelpWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdHelpWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdHelpWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdHelpWindow(myself, "Help", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
