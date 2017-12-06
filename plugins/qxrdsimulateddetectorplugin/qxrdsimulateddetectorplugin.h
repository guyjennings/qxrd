#ifndef QXRDSIMULATEDDETECTORPLUGIN_H
#define QXRDSIMULATEDDETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"
#include <QTimer>

class QxrdSimulatedDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "simulatedDetector.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdSimulatedDetectorPlugin();

    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcquisitionWPtr acq);
};

#endif // QXRDSIMULATEDDETECTORPLUGIN_H
