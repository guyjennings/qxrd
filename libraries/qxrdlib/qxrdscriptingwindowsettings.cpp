#include "qxrdscriptingwindowsettings.h"
#include "qxrdscriptingwindow.h"

QxrdScriptingWindowSettings::QxrdScriptingWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdScriptingWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdScriptingWindow("Scripting", m_Application, m_Experiment));

  return m_Window;
}
