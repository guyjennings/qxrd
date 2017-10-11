#ifndef QCEPIMAGEDATASPREADSHEETMODEL_H
#define QCEPIMAGEDATASPREADSHEETMODEL_H

#include "qceplib_global.h"
#include "qcepimagedata.h"

#include "qcepspreadsheetmodel.h"

class QCEP_EXPORT QcepImageDataSpreadsheetModel : public QcepSpreadsheetModel
{
  Q_OBJECT

public:
  QcepImageDataSpreadsheetModel(QWeakPointer< QcepImageDataBase > img);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  QWeakPointer < QcepImageDataBase > m_Image;
};

#endif // QCEPIMAGEDATASPREADSHEETMODEL_H
