#include "qxrddexelaplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrddexeladriver.h"
#include "qxrddexelasettings.h"

QxrdDexelaPlugin::QxrdDexelaPlugin()
{
}

QString QxrdDexelaPlugin::name() const
{
  return "Dexela Detector Plugin";
}

QxrdDetectorDriverPtr QxrdDexelaPlugin::createDetector(QString name,
                                                               QxrdDetectorSettingsWPtr det,
                                                               QxrdExperimentWPtr expt,
                                                               QxrdAcquisitionWPtr acq)
{
  QxrdDexelaSettingsWPtr set = qSharedPointerDynamicCast<QxrdDexelaSettings>(det);

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdDexelaDriver(name, set, expt, acq));

  return res;
}
