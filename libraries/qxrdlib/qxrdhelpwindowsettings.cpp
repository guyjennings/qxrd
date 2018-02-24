#include "qxrdhelpwindowsettings.h"
#include "qxrdhelpwindow.h"
#include <QThread>

QxrdHelpWindowSettings::QxrdHelpWindowSettings(QString name, QString desc)
  : inherited(name, desc)
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
        new QxrdHelpWindow("Help"));

  return m_Window;
}
