#ifndef QXRDINTEGRATEDDATASPREADSHEETMODEL_H
#define QXRDINTEGRATEDDATASPREADSHEETMODEL_H

#include "qxrdintegrateddata-ptr.h"
#include "qcepspreadsheetmodel.h"

class QxrdIntegratedDataSpreadsheetModel : public QcepSpreadsheetModel
{
public:
  QxrdIntegratedDataSpreadsheetModel(QxrdIntegratedDataPtr integ);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QxrdIntegratedDataPtr m_Integrated;
};

#endif // QXRDINTEGRATEDDATASPREADSHEETMODEL_H
