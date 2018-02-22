#include "qxrdpilatusplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdpilatusdriver.h"

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

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdPilatusDriver(name, plugin, det, expt, acq));

  return res;
}
