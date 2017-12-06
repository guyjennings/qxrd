#ifndef QXRDDETECTORPLUGININTERFACE_H
#define QXRDDETECTORPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include <QtPlugin>
#include "qxrdnamedplugininterface.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectordriver-ptr.h"

class QXRD_EXPORT QxrdDetectorPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdDetectorPluginInterface() {}

  virtual QxrdDetectorDriverPtr createDetector(QString name,
                                               QxrdDetectorSettingsWPtr det,
                                               QxrdExperimentWPtr expt,
                                               QxrdAcquisitionWPtr acq) = 0;
};

#define DetectorPluginInterface_iid "gov.anl.aps.cep.Qxrd.DetectorInterface/2.0"
Q_DECLARE_INTERFACE(QxrdDetectorPluginInterface, DetectorPluginInterface_iid)

#endif // QXRDDETECTORPLUGININTERFACE_H
