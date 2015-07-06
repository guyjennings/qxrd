#ifndef QCEPDATACOLUMNSCANSPREADSHEETMODEL_H
#define QCEPDATACOLUMNSCANSPREADSHEETMODEL_H

#include "qcepdatacolumnscan-ptr.h"
#include <QAbstractTableModel>

class QcepDataColumnScanSpreadsheetModel : public QAbstractTableModel
{
public:
  QcepDataColumnScanSpreadsheetModel(QcepDataColumnScanPtr scan);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QcepDataColumnScanPtr m_Scan;
};

#endif // QCEPDATACOLUMNSCANSPREADSHEETMODEL_H
