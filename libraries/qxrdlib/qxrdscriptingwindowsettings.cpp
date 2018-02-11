#include "qxrdscriptingwindowsettings.h"
#include "qxrdscriptingwindow.h"
#include "qxrdscriptingwindowsettings.h"
#include <QThread>

QxrdScriptingWindowSettings::QxrdScriptingWindowSettings(QString name, QString desc)
  : QxrdMainWindowSettings(name, desc)
{

}

QxrdMainWindowPtr QxrdScriptingWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdScriptingWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdScriptingWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdScriptingWindow(myself,
                                get_Name(),
                                m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
