#ifndef QCEPDATAARRAYSPREADSHEETMODEL_H
#define QCEPDATAARRAYSPREADSHEETMODEL_H

#include "qcepdataarray-ptr.h"
#include <QAbstractTableModel>

class QcepDataArraySpreadsheetModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepDataArraySpreadsheetModel(QcepDataArrayPtr array);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
  QcepDataArrayPtr m_Array;
};

#endif // QCEPDATAARRAYSPREADSHEETMODEL_H
