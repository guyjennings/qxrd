#include "qxrddexeladetectorplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrddexeladetectordriver.h"

QxrdDexelaDetectorPlugin::QxrdDexelaDetectorPlugin()
{
}

QString QxrdDexelaDetectorPlugin::name() const
{
  return "Dexela Detector Plugin";
}

QxrdDetectorDriverPtr QxrdDexelaDetectorPlugin::createDetector(QString name,
                                                               QxrdDetectorSettingsPtr det,
                                                               QxrdExperimentPtr expt,
                                                               QxrdAcquisitionPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdDexelaDetectorDriver(name, det, expt, acq));

  return res;
}
