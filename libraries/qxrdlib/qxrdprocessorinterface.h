#ifndef QXRDPROCESSORINTERFACE_H
#define QXRDPROCESSORINTERFACE_H

#include "qxrdlib_global.h"
#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QXRD_EXPORT QxrdProcessorInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdProcessorInterface() {}
};

#define QxrdProcessorInterface_iid "gov.anl.aps.cep.Qxrd.ProcessorInterface/1.0"
Q_DECLARE_INTERFACE(QxrdProcessorInterface, QxrdProcessorInterface_iid)

#endif // QXRDPROCESSORINTERFACE_H
