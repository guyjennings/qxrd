#ifndef QCEPDATAEXPORTCOMMAND_H
#define QCEPDATAEXPORTCOMMAND_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include <QModelIndexList>

class QcepDataExportCommand : public QcepObject
{
public:
  QcepDataExportCommand(QcepDatasetModelPtr model, const QModelIndexList &idx);

private:
  QcepDatasetModelPtr m_Model;
  QModelIndexList     m_Indexes;
};

#endif // QCEPDATAEXPORTCOMMAND_H
