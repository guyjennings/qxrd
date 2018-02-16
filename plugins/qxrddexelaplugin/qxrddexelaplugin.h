#ifndef QXRDDEXELAPLUGIN_H
#define QXRDDEXELAPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugininterface.h"

class BusScanner;

class QxrdDexelaPlugin : public QcepObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "dexela.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdDetectorPluginInterface)

private:
  typedef QcepObject inherited;

public:
  QxrdDexelaPlugin();

  void initialize(QcepObjectWPtr parent);
  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq);

private:
  BusScanner *m_BusScanner;
  int         m_NDevs;
};

#endif // QXRDDEXELAPLUGIN_H
