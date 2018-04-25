#include "qxrdpilatusplugin.h"
#include "qxrdpilatusdriver.h"
#include "qxrdpilatussettings.h"
#include "qxrdpilatusremote.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
  : inherited("pilatusPlugin")
{
}

void QxrdPilatusPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdPilatusPlugin*>("QxrdPilatusPlugin*");
  qRegisterMetaType<QxrdPilatusDriver*>("QxrdPilatusDriver*");
  qRegisterMetaType<QxrdPilatusRemote*>("QxrdPilatusRemote*");
}

void QxrdPilatusPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdPilatusPlugin::initialize");
}

QString QxrdPilatusPlugin::name() const
{
  return "Pilatus Detector";
}

QxrdDetectorDriverPtr QxrdPilatusPlugin::createDetector(QString name,
                                                        QxrdDetectorSettingsWPtr det,
                                                        QxrdExperimentWPtr expt,
                                                        QxrdAcqCommonWPtr acq)
{
  QxrdPilatusPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdPilatusPlugin>(sharedFromThis()));

  QxrdPilatusSettingsPtr set(
        qSharedPointerDynamicCast<QxrdPilatusSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(NEWPTR(QxrdPilatusDriver(name, plugin, set, expt, acq)));

  return res;
}
