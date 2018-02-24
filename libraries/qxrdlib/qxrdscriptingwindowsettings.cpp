#include "qxrdscriptingwindowsettings.h"
#include "qxrdscriptingwindow.h"
#include "qxrdscriptingwindowsettings.h"
#include <QThread>

QxrdScriptingWindowSettings::QxrdScriptingWindowSettings(QString name, QString desc)
  : inherited(name, desc)
{

}

void QxrdScriptingWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdScriptingWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdScriptingWindowSettingsPtr myself = qSharedPointerDynamicCast<QxrdScriptingWindowSettings>(sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdScriptingWindow(get_Name()));

  return m_Window;
}
