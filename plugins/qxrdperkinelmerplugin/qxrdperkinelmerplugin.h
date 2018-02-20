#ifndef QXRDPERKINELMERPLUGIN_H
#define QXRDPERKINELMERPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugininterface.h"

class QxrdPerkinElmerPlugin : public QcepObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPluginInterface_iid FILE "pe.json")
  Q_INTERFACES(QxrdDetectorPluginInterface)

private:
  typedef QcepObject inherited;

public:
  QxrdPerkinElmerPlugin();

  void initialize(QcepObjectWPtr parent);
  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq);
};

#endif // QXRDPERKINELMERPLUGIN_H
