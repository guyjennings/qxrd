#ifndef QCEPDATAGROUPSPREADSHEETMODEL_H
#define QCEPDATAGROUPSPREADSHEETMODEL_H

#include "qcepdatagroup-ptr.h"
#include "qcepspreadsheetmodel.h"

class QcepDataGroupSpreadsheetModel : public QcepSpreadsheetModel
{
public:
  QcepDataGroupSpreadsheetModel(QcepDataGroupWPtr group);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QcepDataGroupWPtr m_Group;
};

#endif // QCEPDATAGROUPSPREADSHEETMODEL_H
