#ifndef QXRDDETECTORPLUGIN_H
#define QXRDDETECTORPLUGIN_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrddetectordriver-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"

class QXRD_EXPORT QxrdDetectorPlugin : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdDetectorPlugin(QString name);

  virtual void registerMetaTypes() = 0;

  virtual QxrdDetectorDriverPtr createDetector(QString name,
                                               QxrdDetectorSettingsWPtr det,
                                               QxrdExperimentWPtr expt,
                                               QxrdAcqCommonWPtr acq) = 0;

  virtual QString name() const = 0;
};

#define DetectorPlugin_iid "gov.anl.aps.cep.Qxrd.DetectorPlugin"
Q_DECLARE_INTERFACE(QxrdDetectorPlugin, DetectorPlugin_iid)

#endif // QXRDDETECTORPLUGIN_H
