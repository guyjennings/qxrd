#ifndef QXRDSIMULATEDDETECTORPLUGIN_H
#define QXRDSIMULATEDDETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"
#include <QTimer>

class QxrdFileWatcherPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "filewatcher.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

public:
    QxrdFileWatcherPlugin();

    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcquisitionWPtr acq);
};

#endif // QXRDSIMULATEDDETECTORPLUGIN_H
