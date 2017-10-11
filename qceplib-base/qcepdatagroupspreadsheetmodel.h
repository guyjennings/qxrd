#ifndef QCEPDATAGROUPSPREADSHEETMODEL_H
#define QCEPDATAGROUPSPREADSHEETMODEL_H

#include "qceplib_global.h"
#include "qcepdatagroup-ptr.h"
#include "qcepspreadsheetmodel.h"

class QCEP_EXPORT QcepDataGroupSpreadsheetModel : public QcepSpreadsheetModel
{
  Q_OBJECT

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
