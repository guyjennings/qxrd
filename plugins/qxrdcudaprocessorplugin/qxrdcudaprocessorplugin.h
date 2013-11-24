#ifndef QXRDCUDAPROCESSORPLUGIN_H
#define QXRDCUDAPROCESSORPLUGIN_H

#include <QObject>
#include "qxrdprocessorinterface.h"

class QxrdCudaProcessorPlugin : public QObject, public QxrdProcessorInterface
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID QxrdProcessorInterface_iid FILE "cuda.json")
#endif
  Q_INTERFACES(QxrdProcessorInterface)

public:
    QxrdCudaProcessorPlugin();

    QString name() const;
};

#endif // QXRDCUDAPROCESSORPLUGIN_H
