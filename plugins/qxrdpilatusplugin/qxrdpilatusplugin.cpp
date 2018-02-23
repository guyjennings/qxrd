#include "qxrdpilatusplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdpilatusdriver.h"
#include "qxrdpilatussettings.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
  : inherited("pilatusPlugin")
{
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
      QxrdDetectorDriverPtr(new QxrdPilatusDriver(name, plugin, set, expt, acq));

  return res;
}
