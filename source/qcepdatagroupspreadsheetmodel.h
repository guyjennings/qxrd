#ifndef QCEPDATAGROUPSPREADSHEETMODEL_H
#define QCEPDATAGROUPSPREADSHEETMODEL_H

#include "qcepdatagroup-ptr.h"
#include <QAbstractTableModel>

class QcepDataGroupSpreadsheetModel : public QAbstractTableModel
{
public:
  QcepDataGroupSpreadsheetModel(QcepDataGroupPtr group);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QcepDataGroupPtr m_Group;
};

#endif // QCEPDATAGROUPSPREADSHEETMODEL_H
