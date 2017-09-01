#ifndef QXRDSPECREMOTECOMMAND_H
#define QXRDSPECREMOTECOMMAND_H

#include <QObject>
#include "qxrdexperimentcommand.h"

class QxrdSpecRemoteCommand : public QxrdExperimentCommand
{
public:
  QxrdSpecRemoteCommand(QString name, QcepExperimentPtr expt);
};

#endif // QXRDSPECREMOTECOMMAND_H
