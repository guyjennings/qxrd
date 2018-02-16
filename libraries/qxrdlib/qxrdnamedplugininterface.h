#ifndef QXRDNAMEDPLUGININTERFACE_H
#define QXRDNAMEDPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include <QtPlugin>
#include "qcepobject-ptr.h"

class QXRD_EXPORT QxrdNamedPluginInterface
{
public:
  virtual ~QxrdNamedPluginInterface() {}

  virtual QString name() const = 0;
  virtual void initialize(QcepObjectWPtr parent) = 0;
};

Q_DECLARE_INTERFACE(QxrdNamedPluginInterface, "gov.anl.aps.cep.Qxrd.NamedPluginInterface/1.0")

#endif // QXRDNAMEDPLUGININTERFACE_H
