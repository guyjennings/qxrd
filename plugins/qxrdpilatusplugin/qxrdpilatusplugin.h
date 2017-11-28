#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPilatusPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "pilatus.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

public:
  QxrdPilatusPlugin();

  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsPtr det,
                                       QxrdExperimentPtr expt,
                                       QxrdAcquisitionPtr acq);
};

#endif // QXRDPILATUSPLUGIN_H
