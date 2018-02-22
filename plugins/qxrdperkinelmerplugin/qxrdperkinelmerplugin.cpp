#include "qxrdperkinelmerplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdperkinelmerdriver.h"

QxrdPerkinElmerPlugin::QxrdPerkinElmerPlugin()
  : inherited("perkinElmerPlugin")
{
}

void QxrdPerkinElmerPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdPerkinElmerPlugin::initialize");
}

QString QxrdPerkinElmerPlugin::name() const
{
  return "Perkin Elmer XRD Detector";
}

QxrdDetectorDriverPtr QxrdPerkinElmerPlugin::createDetector(QString name,
                                                            QxrdDetectorSettingsWPtr det,
                                                            QxrdExperimentWPtr expt,
                                                            QxrdAcqCommonWPtr acq)
{
  QxrdPerkinElmerPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdPerkinElmerPlugin>(sharedFromThis()));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdPerkinElmerDriver(name, plugin, det, expt, acq));

  return res;
}
