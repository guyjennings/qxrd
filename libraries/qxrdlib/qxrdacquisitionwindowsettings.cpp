#include "qxrdacquisitionwindowsettings.h"

QxrdAcquisitionWindowSettings::QxrdAcquisitionWindowSettings(QString name,
                                                             QxrdApplicationPtr app,
                                                             QxrdExperimentPtr expt)
  : QxrdMainWindowSettings(name, app, expt)
{
}
