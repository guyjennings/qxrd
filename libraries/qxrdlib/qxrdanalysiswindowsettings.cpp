#include "qxrdanalysiswindowsettings.h"
#include "qxrdanalysiswindow.h"
#include <QThread>

QxrdAnalysisWindowSettings::QxrdAnalysisWindowSettings(QString name)
  : inherited(name, "Analysis Window")
{

}

void QxrdAnalysisWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepMainWindowPtr QxrdAnalysisWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        NEWPTR(QxrdAnalysisWindow("Analysis")));

  return m_Window;
}
