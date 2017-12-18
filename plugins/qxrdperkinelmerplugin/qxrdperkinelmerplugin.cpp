#include "qxrdperkinelmerplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdperkinelmerdriver.h"

QxrdPerkinElmerPlugin::QxrdPerkinElmerPlugin()
{
}

QString QxrdPerkinElmerPlugin::name() const
{
  return "Perkin Elmer XRD Detector";
}

QxrdDetectorDriverPtr QxrdPerkinElmerPlugin::createDetector(QString name,
                                                            QxrdDetectorSettingsWPtr det,
                                                            QxrdExperimentWPtr expt,
                                                            QxrdAcquisitionWPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdPerkinElmerDriver(name, det, expt, acq));

  return res;
}
