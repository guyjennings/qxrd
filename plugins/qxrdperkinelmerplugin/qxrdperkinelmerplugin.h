#ifndef QXRDPERKINELMERPLUGIN_H
#define QXRDPERKINELMERPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPerkinElmerPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "pe.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

public:
  QxrdPerkinElmerPlugin();

  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcquisitionWPtr acq);
};

#endif // QXRDPERKINELMERPLUGIN_H
