#ifndef QXRDDEXELADETECTORPLUGIN_H
#define QXRDDEXELADETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdDexelaDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "dexelaDetector.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
    QxrdDexelaDetectorPlugin();

    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcquisitionWPtr acq);
};

#endif // QXRDDEXELADETECTORPLUGIN_H
