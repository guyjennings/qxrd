#ifndef QXRDIMAGEDATASPREADSHEETMODEL_H
#define QXRDIMAGEDATASPREADSHEETMODEL_H

#include "qcepimagedata.h"

#include "qcepspreadsheetmodel.h"

class QxrdImageDataSpreadsheetModel : public QcepSpreadsheetModel
{
public:
  QxrdImageDataSpreadsheetModel(QSharedPointer< QcepImageDataBase > img);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
  QSharedPointer < QcepImageDataBase > m_Image;
};

#endif // QXRDIMAGEDATASPREADSHEETMODEL_H
