#ifndef QXRDSIMULATEDPLUGIN_H
#define QXRDSIMULATEDPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugin.h"

class QxrdSimulatedPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "simulated.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

public:
    QxrdSimulatedPlugin();

    void registerMetaTypes();

    void initialize(QcepObjectWPtr parent);
    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcqCommonWPtr acq);
};

Q_DECLARE_METATYPE(QxrdSimulatedPlugin*)

#endif // QXRDSIMULATEDPLUGIN_H
