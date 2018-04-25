#include "qxrdperkinelmerplugin.h"
#include "qxrdperkinelmerdriver.h"
#include "qxrdperkinelmersettings.h"

QxrdPerkinElmerPlugin::QxrdPerkinElmerPlugin()
  : inherited("perkinElmerPlugin")
{
}

void QxrdPerkinElmerPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdPerkinElmerPlugin::initialize");
}

void QxrdPerkinElmerPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdPerkinElmerPlugin*>("QxrdPerkinElmerPlugin*");
  qRegisterMetaType<QxrdPerkinElmerDriver*>("QxrdPerkinElmerDriver*");
}
QString QxrdPerkinElmerPlugin::name() const
{
  return "Perkin Elmer Detector";
}

QxrdDetectorDriverPtr QxrdPerkinElmerPlugin::createDetector(QString name,
                                                            QxrdDetectorSettingsWPtr det,
                                                            QxrdExperimentWPtr expt,
                                                            QxrdAcqCommonWPtr acq)
{
  QxrdPerkinElmerPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdPerkinElmerPlugin>(sharedFromThis()));

  QxrdPerkinElmerSettingsPtr set(
        qSharedPointerDynamicCast<QxrdPerkinElmerSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(NEWPTR(QxrdPerkinElmerDriver(name, plugin, set, expt, acq)));

  return res;
}
