#include "qxrdfilewatcherplugin.h"
#include "qxrdfilewatcherdriver.h"
#include "qxrdfilewatchersettings.h"

QxrdFileWatcherPlugin::QxrdFileWatcherPlugin()
  : inherited("fileWatcherPlugin")
{
}

void QxrdFileWatcherPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdFileWatcherPlugin::initialize");
}

QString QxrdFileWatcherPlugin::name() const
{
  return "File Watcher Plugin";
}

QxrdDetectorDriverPtr QxrdFileWatcherPlugin::createDetector(QString name,
                                                            QxrdDetectorSettingsWPtr det,
                                                            QxrdExperimentWPtr expt,
                                                            QxrdAcqCommonWPtr acq)
{
  QxrdFileWatcherPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdFileWatcherPlugin>(sharedFromThis()));

  QxrdFileWatcherSettingsPtr set(
        qSharedPointerDynamicCast<QxrdFileWatcherSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdFileWatcherDriver(name, plugin, set, expt, acq));

  return res;
}
