#ifndef QXRDAREADETECTORPLUGIN_H
#define QXRDAREADETECTORPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugininterface.h"

class QxrdAreaDetectorPlugin : public QcepObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "areaDetector.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

private:
  typedef QcepObject inherited;

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
