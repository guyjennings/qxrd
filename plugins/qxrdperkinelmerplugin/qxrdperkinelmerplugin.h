#ifndef QXRDPERKINELMERPLUGIN_H
#define QXRDPERKINELMERPLUGIN_H

#include "qcepobject.h"
#include "qxrddetectorplugin.h"

class QxrdPerkinElmerPlugin :
    public QxrdDetectorPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID DetectorPlugin_iid FILE "pe.json")
  Q_INTERFACES(QxrdDetectorPlugin)

private:
  typedef QxrdDetectorPlugin inherited;

public:
  QxrdPerkinElmerPlugin();

  void initialize(QcepObjectWPtr parent);

  void registerMetaTypes();

  QString name() const;

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq);
};

Q_DECLARE_METATYPE(QxrdPerkinElmerPlugin*)

#endif // QXRDPERKINELMERPLUGIN_H
