#include "qxrdareadetectorplugin.h"
#include "qxrdareadetectordriver.h"
#include "qxrdareadetectorsettings.h"

QxrdAreaDetectorPlugin::QxrdAreaDetectorPlugin()
  : inherited("areaDetectorPlugin")
{
}

void QxrdAreaDetectorPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdAreaDetectorPlugin::initialize");
}

QString QxrdAreaDetectorPlugin::name() const
{
  return "EPICS Area Detector Plugin";
}

QxrdDetectorDriverPtr QxrdAreaDetectorPlugin::createDetector(QString name,
                                                             QxrdDetectorSettingsWPtr det,
                                                             QxrdExperimentWPtr expt,
                                                             QxrdAcqCommonWPtr acq)
{
  QxrdAreaDetectorPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdAreaDetectorPlugin>(sharedFromThis()));

  QxrdAreaDetectorSettingsPtr set(
        qSharedPointerDynamicCast<QxrdAreaDetectorSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdAreaDetectorDriver(name, plugin, set, expt, acq));

  return res;
}
