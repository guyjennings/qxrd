#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPilatusPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "pilatus.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

public:
  QxrdPilatusPlugin();

  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcquisitionWPtr acq);
};

#endif // QXRDPILATUSPLUGIN_H
