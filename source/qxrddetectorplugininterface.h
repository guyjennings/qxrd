#ifndef QXRDDETECTORPLUGININTERFACE_H
#define QXRDDETECTORPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdDetectorPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdDetectorPluginInterface() {}
};

Q_DECLARE_INTERFACE(QxrdDetectorPluginInterface, "gov.anl.aps.cep.Qxrd.DetectorInterface/1.0")

#endif // QXRDDETECTORPLUGININTERFACE_H
