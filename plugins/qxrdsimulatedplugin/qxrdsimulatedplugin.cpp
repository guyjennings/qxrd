#include "qxrdsimulatedplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdsimulateddriver.h"
#include "qxrdsimulatedsettings.h"

QxrdSimulatedPlugin::QxrdSimulatedPlugin()
{
}

QString QxrdSimulatedPlugin::name() const
{
  return "Simulated Detector Plugin";
}

QxrdDetectorDriverPtr QxrdSimulatedPlugin::createDetector(QString name,
                                                          QxrdDetectorSettingsWPtr det,
                                                          QxrdExperimentWPtr expt,
                                                          QxrdAcquisitionWPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdSimulatedDriver(name, det, expt, acq));

  return res;
}
