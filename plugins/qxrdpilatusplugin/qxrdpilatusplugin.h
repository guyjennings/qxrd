#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugininterface.h"

class QxrdPilatusPlugin : public QcepObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "pilatus.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

private:
  typedef QcepObject inherited;

public:
  QxrdPilatusPlugin();

  void initialize(QcepObjectWPtr parent);
  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq);
};

#endif // QXRDPILATUSPLUGIN_H
