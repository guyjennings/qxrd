#ifndef QXRDDETECTORPLUGININTERFACE_H
#define QXRDDETECTORPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include <QtPlugin>
#include "qxrddetectorsettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrddetectordriver-ptr.h"
#include "qcepobject-ptr.h"

class QXRD_EXPORT QxrdDetectorPluginInterface
{
public:
  virtual ~QxrdDetectorPluginInterface() {}

  virtual QxrdDetectorDriverPtr createDetector(QString name,
                                               QxrdDetectorSettingsWPtr det,
                                               QxrdExperimentWPtr expt,
                                               QxrdAcqCommonWPtr acq) = 0;

  virtual QString name() const = 0;
  virtual void initialize(QcepObjectWPtr parent) = 0;
};

#define DetectorPluginInterface_iid "gov.anl.aps.cep.Qxrd.DetectorInterface"
Q_DECLARE_INTERFACE(QxrdDetectorPluginInterface, DetectorPluginInterface_iid)

#endif // QXRDDETECTORPLUGININTERFACE_H
