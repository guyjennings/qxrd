#ifndef QXRDAREADETECTORPLUGIN_H
#define QXRDAREADETECTORPLUGIN_H

#include "qxrddetectorplugin.h"

class QxrdAreaDetectorPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "areaDetector.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

public:
    QxrdAreaDetectorPlugin();

    void initialize(QcepObjectWPtr parent);
    QString name() const;

    QxrdDetectorDriverPtr createDetector(QString name,
                                         QxrdDetectorSettingsWPtr det,
                                         QxrdExperimentWPtr expt,
                                         QxrdAcqCommonWPtr acq);
};

#endif // QXRDAREADETECTORPLUGIN_H
