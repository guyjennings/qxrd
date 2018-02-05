#ifndef QCEPDATAARRAYSPREADSHEETMODEL_H
#define QCEPDATAARRAYSPREADSHEETMODEL_H

#include "qceplib_global.h"
#include "qcepdataarray-ptr.h"
#include "qcepspreadsheetmodel.h"

class QCEP_EXPORT QcepDataArraySpreadsheetModel : public QcepSpreadsheetModel
{
  Q_OBJECT

private:
  typedef QcepSpreadsheetModel inherited;

public:
  QcepDataArraySpreadsheetModel(QcepDataArrayWPtr array);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
  QcepDataArrayWPtr m_Array;
};

#endif // QCEPDATAARRAYSPREADSHEETMODEL_H
