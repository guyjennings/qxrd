#include "qxrdhelpwindowsettings.h"
#include "qxrdhelpwindow.h"
#include <QThread>

QxrdHelpWindowSettings::QxrdHelpWindowSettings(QString name)
  : inherited(name, "Help Window")
{

}

void QxrdHelpWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdHelpWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        NEWPTR(QxrdHelpWindow("Help")));

  return m_Window;
}
