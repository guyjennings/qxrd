#ifndef QXRDDETECTORPLUGININTERFACE_H
#define QXRDDETECTORPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QXRD_EXPORT QxrdDetectorPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdDetectorPluginInterface() {}
};

#define DetectorPluginInterface_iid "gov.anl.aps.cep.Qxrd.DetectorInterface/1.0"
Q_DECLARE_INTERFACE(QxrdDetectorPluginInterface, DetectorPluginInterface_iid)

#endif // QXRDDETECTORPLUGININTERFACE_H
