#include "qxrdstartupwindowsettings.h"
#include "qxrdstartupwindow.h"
#include <QThread>

QxrdStartupWindowSettings::QxrdStartupWindowSettings(QString name)
  : inherited(name, "Startup Window")
{

}

void QxrdStartupWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdStartupWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdStartupWindow("startupWindow"),
        &QObject::deleteLater);

  return m_Window;
}
