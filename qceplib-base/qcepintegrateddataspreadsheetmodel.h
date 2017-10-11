#ifndef QCEPINTEGRATEDDATASPREADSHEETMODEL_H
#define QCEPINTEGRATEDDATASPREADSHEETMODEL_H

#include "qceplib_global.h"
#include "qcepintegrateddata-ptr.h"
#include "qcepspreadsheetmodel.h"

class QCEP_EXPORT QcepIntegratedDataSpreadsheetModel : public QcepSpreadsheetModel
{
  Q_OBJECT

public:
  QcepIntegratedDataSpreadsheetModel(QcepIntegratedDataWPtr integ);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QcepIntegratedDataWPtr m_Integrated;
};

#endif // QCEPINTEGRATEDDATASPREADSHEETMODEL_H
