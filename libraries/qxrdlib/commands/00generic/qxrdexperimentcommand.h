#ifndef QXRDEXPERIMENTCOMMAND_H
#define QXRDEXPERIMENTCOMMAND_H

#include <QObject>
#include "qxrdcommand.h"
#include "qcepexperiment-ptr.h"

class QxrdExperimentCommand : public QxrdCommand
{
public:
  QxrdExperimentCommand(QString name, QcepExperimentPtr expt);

protected:
  QcepExperimentWPtr m_Experiment;
};

#endif // QXRDEXPERIMENTCOMMAND_H
