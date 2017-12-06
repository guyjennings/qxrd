#include "qxrdsimulateddetectorplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdsimulateddetectordriver.h"

QxrdSimulatedDetectorPlugin::QxrdSimulatedDetectorPlugin()
{
}

QString QxrdSimulatedDetectorPlugin::name() const
{
  return "Simulated Detector Plugin";
}

QxrdDetectorDriverPtr QxrdSimulatedDetectorPlugin::createDetector(QString name,
                                                                  QxrdDetectorSettingsWPtr det,
                                                                  QxrdExperimentWPtr expt,
                                                                  QxrdAcquisitionWPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdSimulatedDetectorDriver(name, det, expt, acq));

  return res;
}
