#ifndef QCEPDATACOLUMNSCANSPREADSHEETMODEL_H
#define QCEPDATACOLUMNSCANSPREADSHEETMODEL_H

#include "qceplib_global.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepspreadsheetmodel.h"

class QCEP_EXPORT QcepDataColumnScanSpreadsheetModel : public QcepSpreadsheetModel
{
  Q_OBJECT

public:
  QcepDataColumnScanSpreadsheetModel(QcepDataColumnScanWPtr scan);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QcepDataColumnScanWPtr m_Scan;
};

#endif // QCEPDATACOLUMNSCANSPREADSHEETMODEL_H
