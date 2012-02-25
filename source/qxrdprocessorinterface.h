#ifndef QXRDPROCESSORINTERFACE_H
#define QXRDPROCESSORINTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdProcessorInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdProcessorInterface() {}
};

Q_DECLARE_INTERFACE(QxrdProcessorInterface, "gov.anl.aps.cep.Qxrd.ProcessorInterface/1.0")

#endif // QXRDPROCESSORINTERFACE_H
