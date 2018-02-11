#include "qxrdanalysiswindowsettings.h"
#include "qxrdanalysiswindow.h"
#include <QThread>

QxrdAnalysisWindowSettings::QxrdAnalysisWindowSettings(QString name, QString desc)
  : inherited(name, desc)
{

}

void QxrdAnalysisWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QxrdMainWindowPtr QxrdAnalysisWindowSettings::newWindow()
{
  GUI_THREAD_CHECK;

  m_Window =
      QxrdMainWindowPtr(
        new QxrdAnalysisWindow("Analysis",
                               m_Application,
                               m_Experiment,
                               m_Acquisition,
                               m_Processor));

  return m_Window;
}
