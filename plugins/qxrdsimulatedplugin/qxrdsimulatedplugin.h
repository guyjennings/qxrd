#ifndef QXRDSIMULATEDPLUGIN_H
#define QXRDSIMULATEDPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugininterface.h"

class QxrdSimulatedPlugin : public QcepObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "simulated.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

private:
  typedef QcepObject inherited;

public:
    QxrdSimulatedPlugin();

    void initialize(QcepObjectWPtr parent);
    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcqCommonWPtr acq);
};

#endif // QXRDSIMULATEDPLUGIN_H
