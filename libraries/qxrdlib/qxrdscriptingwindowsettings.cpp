#include "qxrdscriptingwindowsettings.h"
#include "qxrdscriptingwindow.h"
#include "qxrdscriptingwindowsettings.h"

QxrdScriptingWindowSettings::QxrdScriptingWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdScriptingWindowSettings::newWindow()
{
  QxrdScriptingWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdScriptingWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdScriptingWindow(myself, "Scripting", m_Application, m_Experiment));

  return m_Window;
}
