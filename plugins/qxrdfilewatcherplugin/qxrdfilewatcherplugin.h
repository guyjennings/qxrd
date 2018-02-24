#ifndef QXRDSIMULATEDDETECTORPLUGIN_H
#define QXRDSIMULATEDDETECTORPLUGIN_H

#include "qcepobject.h"
#include <QTimer>
#include "qxrddetectorplugin.h"

class QxrdFileWatcherPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "filewatcher.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

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
