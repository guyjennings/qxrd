#include "qxrddatasetcommand.h"

QxrdDatasetCommand::QxrdDatasetCommand(QString name, QcepDatasetModelPtr model)
  : QxrdCommand(name),
    m_Model(model)
{

}
