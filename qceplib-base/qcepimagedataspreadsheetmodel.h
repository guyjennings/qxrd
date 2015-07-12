#ifndef QCEPIMAGEDATASPREADSHEETMODEL_H
#define QCEPIMAGEDATASPREADSHEETMODEL_H

#include "qcepimagedata.h"

#include "qcepspreadsheetmodel.h"

class QcepImageDataSpreadsheetModel : public QcepSpreadsheetModel
{
public:
  QcepImageDataSpreadsheetModel(QSharedPointer< QcepImageDataBase > img);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
  QSharedPointer < QcepImageDataBase > m_Image;
};

#endif // QCEPIMAGEDATASPREADSHEETMODEL_H
