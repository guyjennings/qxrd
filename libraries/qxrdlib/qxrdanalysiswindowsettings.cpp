#include "qxrdanalysiswindowsettings.h"
#include "qxrdanalysiswindow.h"

QxrdAnalysisWindowSettings::QxrdAnalysisWindowSettings(QString name)
  : QxrdMainWindowSettings(name)
{

}

QxrdMainWindowPtr QxrdAnalysisWindowSettings::newWindow()
{
  m_Window =
      QxrdMainWindowPtr(
        new QxrdAnalysisWindow("Analysis", m_Application, m_Experiment, m_Acquisition, m_Processor));

  return m_Window;
}
