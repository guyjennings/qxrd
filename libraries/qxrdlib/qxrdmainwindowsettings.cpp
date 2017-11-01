#include "qxrdmainwindowsettings.h"

QxrdMainWindowSettings::QxrdMainWindowSettings(QString name)
  : QcepMainWindowSettings(name)
{
}

void QxrdMainWindowSettings::initialize(QxrdApplicationWPtr app, QxrdExperimentWPtr expt)
{
  m_Application = app;
  m_Experiment  = expt;
}

QxrdMainWindowPtr QxrdMainWindowSettings::window()
{
  return m_Window;
}
