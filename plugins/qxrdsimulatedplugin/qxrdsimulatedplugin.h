#ifndef QXRDSIMULATEDPLUGIN_H
#define QXRDSIMULATEDPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdSimulatedPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "simulated.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdSimulatedPlugin();

    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcquisitionWPtr acq);
};

#endif // QXRDSIMULATEDPLUGIN_H