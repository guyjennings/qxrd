#include "qxrdscriptingwindowsettings.h"
#include "qxrdscriptingwindow.h"
#include "qxrdscriptingwindowsettings.h"
#include <QThread>

QxrdScriptingWindowSettings::QxrdScriptingWindowSettings(QString name)
  : inherited(name, "Scripting Window")
{

}

void QxrdScriptingWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdScriptingWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdScriptingWindow(get_Name()),
        &QObject::deleteLater);

  return m_Window;
}
