#ifndef QXRDDEXELAPLUGIN_H
#define QXRDDEXELAPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class BusScanner;

class QxrdDexelaPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "dexela.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

public:
  QxrdDexelaPlugin();

  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcquisitionWPtr acq);

private:
  BusScanner *m_BusScanner;
  int         m_NDevs;
};

#endif // QXRDDEXELAPLUGIN_H
