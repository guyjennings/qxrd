#include "qxrdalliedvisionplugin.h"
#include "qxrdalliedvisiondriver.h"
#include "qxrdalliedvisionsettings.h"
#include "qxrdalliedvisionplugin-ptr.h"

QxrdAlliedVisionPlugin::QxrdAlliedVisionPlugin()
  : inherited("alliedVisionPlugin")
{
}

void QxrdAlliedVisionPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdAlliedVisionPlugin*>("QxrdAlliedVisionPlugin*");
  qRegisterMetaType<QxrdAlliedVisionDriver*>("QxrdAlliedVisionDriver*");
}

void QxrdAlliedVisionPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdAlliedVisionPlugin::initialize");
}

QString QxrdAlliedVisionPlugin::name() const
{
  return "Allied Vision Detector Plugin";
}

QxrdDetectorDriverPtr QxrdAlliedVisionPlugin::createDetector(QString name,
                                                          QxrdDetectorSettingsWPtr det,
                                                          QxrdExperimentWPtr expt,
                                                          QxrdAcqCommonWPtr acq)
{
  QxrdAlliedVisionPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdAlliedVisionPlugin>(sharedFromThis()));

  QxrdAlliedVisionSettingsPtr set(
        qSharedPointerDynamicCast<QxrdAlliedVisionSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdAlliedVisionDriver(name, plugin, set, expt, acq));

  return res;
}
