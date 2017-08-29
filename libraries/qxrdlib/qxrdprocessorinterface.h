#ifndef QXRDPROCESSORINTERFACE_H
#define QXRDPROCESSORINTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdProcessorInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdProcessorInterface() {}
};

#define QxrdProcessorInterface_iid "gov.anl.aps.cep.Qxrd.ProcessorInterface/1.0"
Q_DECLARE_INTERFACE(QxrdProcessorInterface, QxrdProcessorInterface_iid)

#endif // QXRDPROCESSORINTERFACE_H
