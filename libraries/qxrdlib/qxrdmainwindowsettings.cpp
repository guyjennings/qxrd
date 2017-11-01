#include "qxrdmainwindowsettings.h"

QxrdMainWindowSettings::QxrdMainWindowSettings(QString name,
                                               QxrdApplicationPtr app,
                                               QxrdExperimentPtr expt)
  : QcepMainWindowSettings(name),
    m_Application(app),
    m_Experiment(expt)
{
}
