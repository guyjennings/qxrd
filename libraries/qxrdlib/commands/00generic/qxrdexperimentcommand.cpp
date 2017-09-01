#include "qxrdexperimentcommand.h"

QxrdExperimentCommand::QxrdExperimentCommand(QString name, QcepExperimentPtr expt)
  : QxrdCommand(name),
    m_Experiment(expt)
{

}
