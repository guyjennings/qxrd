#include "qxrdstartupwindowsettings.h"
#include "qxrdstartupwindow.h"
#include <QThread>

QxrdStartupWindowSettings::QxrdStartupWindowSettings(QString name, QString desc)
  : inherited(name, desc)
{

}

void QxrdStartupWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QxrdMainWindowPtr QxrdStartupWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  QxrdStartupWindowSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdStartupWindowSettings>(
        sharedFromThis());

  m_Window =
      QxrdMainWindowPtr(
        new QxrdStartupWindow(myself,
                              "startupWindow",
                              m_Application));

  return m_Window;
}
