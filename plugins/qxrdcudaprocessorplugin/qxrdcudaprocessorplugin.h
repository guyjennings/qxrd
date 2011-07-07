#ifndef QXRDCUDAPROCESSORPLUGIN_H
#define QXRDCUDAPROCESSORPLUGIN_H

#include <QObject>
#include "qxrdprocessorinterface.h"

class QxrdCudaProcessorPlugin : public QObject, public QxrdProcessorInterface
{
  Q_OBJECT
  Q_INTERFACES(QxrdProcessorInterface)

public:
    QxrdCudaProcessorPlugin();

    QString name() const;
};

#endif // QXRDCUDAPROCESSORPLUGIN_H
