#ifndef QCEPDATACOLUMNSPREADSHEETMODEL_H
#define QCEPDATACOLUMNSPREADSHEETMODEL_H

#include "qcepdatacolumn-ptr.h"
#include "qcepspreadsheetmodel.h"

class QcepDataColumnSpreadsheetModel : public QcepSpreadsheetModel
{
public:
  QcepDataColumnSpreadsheetModel(QcepDataColumnWPtr col);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QcepDataColumnWPtr m_Column;
};

#endif // QCEPDATACOLUMNSPREADSHEETMODEL_H
