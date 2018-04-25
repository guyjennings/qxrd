#include "qxrdsimulatedplugin.h"
#include "qxrdsimulateddriver.h"
#include "qxrdsimulatedsettings.h"
#include "qxrdsimulatedplugin-ptr.h"

QxrdSimulatedPlugin::QxrdSimulatedPlugin()
  : inherited("simulatedPlugin")
{
}

void QxrdSimulatedPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdSimulatedPlugin*>("QxrdSimulatedPlugin*");
  qRegisterMetaType<QxrdSimulatedDriver*>("QxrdSimulatedDriver*");
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
  QxrdSimulatedPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdSimulatedPlugin>(sharedFromThis()));

  QxrdSimulatedSettingsPtr set(
        qSharedPointerDynamicCast<QxrdSimulatedSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(NEWPTR(QxrdSimulatedDriver(name, plugin, set, expt, acq)));

  return res;
}
