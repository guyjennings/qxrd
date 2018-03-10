#ifndef QXRDDEXELAPLUGIN_H
#define QXRDDEXELAPLUGIN_H

#include "qcepobject.h"
#include "BusScanner.h"
#include "qxrddetectorplugin.h"

class QxrdDexelaPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "dexela.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

public:
  QxrdDexelaPlugin();

  void registerMetaTypes();

  void initialize(QcepObjectWPtr parent);
  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq);

  int     deviceCount();
  DevInfo device(int n);

private:
  BusScanner *m_BusScanner;
  int         m_NDevs;
};

Q_DECLARE_METATYPE(QxrdDexelaPlugin*)

#endif // QXRDDEXELAPLUGIN_H
