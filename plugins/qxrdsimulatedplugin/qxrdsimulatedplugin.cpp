#include "qxrdsimulatedplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdsimulateddriver.h"
#include "qxrdsimulatedsettings.h"

QxrdSimulatedPlugin::QxrdSimulatedPlugin()
  : inherited("simulatedPlugin")
{
}

void QxrdSimulatedPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdSimulatedPlugin::initialize");
}

QString QxrdSimulatedPlugin::name() const
{
  return "Simulated Detector Plugin";
}

QxrdDetectorDriverPtr QxrdSimulatedPlugin::createDetector(QString name,
                                                          QxrdDetectorSettingsWPtr det,
                                                          QxrdExperimentWPtr expt,
                                                          QxrdAcqCommonWPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdSimulatedDriver(name, det, expt, acq));

  return res;
}
