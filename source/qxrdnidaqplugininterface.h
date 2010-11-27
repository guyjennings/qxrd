#ifndef QXRDNIDAQPLUGININTERFACE_H
#define QXRDNIDAQPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdNIDAQPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdNIDAQPluginInterface() {}
};

Q_DECLARE_INTERFACE(QxrdNIDAQPluginInterface, "gov.anl.aps.cep.Qxrd.NIDAQInterface/1.0");

#endif // QXRDNIDAQPLUGININTERFACE_H
