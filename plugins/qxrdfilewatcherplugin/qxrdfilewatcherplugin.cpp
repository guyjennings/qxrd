#include "qxrdfilewatcherplugin.h"
#include "qxrddetectorinterface.h"
#include "qxrdfilewatcherdriver.h"

QxrdFileWatcherPlugin::QxrdFileWatcherPlugin()
{
}

QString QxrdFileWatcherPlugin::name() const
{
  return "Simulated Detector Plugin";
}

QxrdDetectorDriverPtr QxrdFileWatcherPlugin::createDetector(QString name,
                                                            QxrdDetectorSettingsWPtr det,
                                                            QxrdExperimentWPtr expt,
                                                            QxrdAcqCommonWPtr acq)
{
  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdFileWatcherDriver(name, det, expt, acq));

  return res;
}
