#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugin.h"

class QxrdPilatusPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "pilatus.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

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
