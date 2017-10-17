#ifndef QXRDDETECTORPLUGININTERFACE_H
#define QXRDDETECTORPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include <QtPlugin>
#include "qxrdnamedplugininterface.h"
#include "qxrddetectorinterface-ptr.h"

class QXRD_EXPORT QxrdDetectorPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdDetectorPluginInterface() {}

  virtual QxrdDetectorInterfacePtr createDetector(QString name) = 0;
};

#define DetectorPluginInterface_iid "gov.anl.aps.cep.Qxrd.DetectorInterface/1.0"
Q_DECLARE_INTERFACE(QxrdDetectorPluginInterface, DetectorPluginInterface_iid)

#endif // QXRDDETECTORPLUGININTERFACE_H
