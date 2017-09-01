#ifndef QXRDDATASETCOMMAND_H
#define QXRDDATASETCOMMAND_H

#include <QObject>
#include "qxrdcommand.h"
#include "qcepdatasetmodel-ptr.h"

class QxrdDatasetCommand : public QxrdCommand
{
public:
  QxrdDatasetCommand(QString name, QcepDatasetModelPtr model);

protected:
  QcepDatasetModelWPtr m_Model;
};

#endif // QXRDDATASETCOMMAND_H
