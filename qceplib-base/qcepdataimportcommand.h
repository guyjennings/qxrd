#ifndef QCEPDATAIMPORTCOMMAND_H
#define QCEPDATAIMPORTCOMMAND_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepdatasetmodel-ptr.h"
#include <QModelIndexList>

class QcepDataImportCommand : public QcepObject
{
public:
  QcepDataImportCommand(QcepDatasetModelPtr model, const QModelIndexList &idx);

private:
  QcepDatasetModelPtr m_Model;
  QModelIndexList     m_Indexes;
};

#endif // QCEPDATAIMPORTCOMMAND_H
