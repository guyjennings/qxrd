#ifndef QXRDCUDAPROCESSORPLUGIN_H
#define QXRDCUDAPROCESSORPLUGIN_H

#include <QObject>
#include "qxrdprocessorinterface.h"

class QxrdCudaProcessorPlugin : public QObject, public QxrdProcessorInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QxrdProcessorInterface_iid FILE "cuda.json")
  Q_INTERFACES(QxrdNamedPluginInterface QxrdProcessorInterface)

public:
    QxrdCudaProcessorPlugin();

    QString name() const;
};

#endif // QXRDCUDAPROCESSORPLUGIN_H
