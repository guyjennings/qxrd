#ifndef QXRDSIMULATEDDETECTORPLUGIN_H
#define QXRDSIMULATEDDETECTORPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugininterface.h"
#include <QTimer>

class QxrdFileWatcherPlugin : public QcepObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "filewatcher.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

private:
  typedef QcepObject inherited;

public:
    QxrdFileWatcherPlugin();

    void initialize(QcepObjectWPtr parent);
    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcqCommonWPtr acq);
};

#endif // QXRDSIMULATEDDETECTORPLUGIN_H
