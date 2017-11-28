#include "qxrdpilatusplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdpilatusdriver.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
{
}

QString QxrdPilatusPlugin::name() const
{
  return "Pilatus Detector";
}

QxrdDetectorDriverPtr QxrdPilatusPlugin::createDetector(QString name,
                                                        QxrdDetectorSettingsPtr det,
                                                        QxrdExperimentPtr expt,
                                                        QxrdAcquisitionPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdPilatusDriver(name, det, expt, acq));

  return res;
}
