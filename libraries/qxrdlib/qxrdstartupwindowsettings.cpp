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

QcepMainWindowPtr QxrdStartupWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdStartupWindow("startupWindow"));

  return m_Window;
}
