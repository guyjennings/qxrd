#include "qxrdsimulatedplugin.h"
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

  QxrdSimulatedSettingsPtr set(
        qSharedPointerDynamicCast<QxrdSimulatedSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdSimulatedDriver(name, plugin, set, expt, acq));

  return res;
}
