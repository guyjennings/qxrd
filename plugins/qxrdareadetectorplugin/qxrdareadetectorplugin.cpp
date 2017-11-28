#include "qxrdareadetectorplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdareadetectordriver.h"

QxrdAreaDetectorPlugin::QxrdAreaDetectorPlugin()
{
}

QString QxrdAreaDetectorPlugin::name() const
{
  return "EPICS Area Detector Plugin";
}

QxrdDetectorDriverPtr QxrdAreaDetectorPlugin::createDetector(QString name,
                                                             QxrdDetectorSettingsPtr det,
                                                             QxrdExperimentPtr expt,
                                                             QxrdAcquisitionPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdAreaDetectorDriver(name, det, expt, acq));

  return res;
}
