#include "qxrdsimulatedplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdsimulateddriver.h"
#include "qxrdsimulatedsettings.h"
#include "qxrdsimulatedplugin-ptr.h"

QxrdSimulatedPlugin::QxrdSimulatedPlugin()
  : inherited("simulatedPlugin")
{
}

void QxrdSimulatedPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QcepObjectWPtr me = sharedFromThis();

  QSharedPointer<QxrdSimulatedPlugin> myself =
      qSharedPointerDynamicCast<QxrdSimulatedPlugin>(me);

  if (myself) {
    printMessage("QxrdSimulatedPlugin::myself != NULL");
  }

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
  QxrdSimulatedPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdSimulatedPlugin>(sharedFromThis()));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdSimulatedDriver(name, plugin, det, expt, acq));

  return res;
}
