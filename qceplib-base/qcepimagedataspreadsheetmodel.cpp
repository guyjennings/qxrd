#include "qcepimagedataspreadsheetmodel.h"
#include <QModelIndex>

QcepImageDataSpreadsheetModel::QcepImageDataSpreadsheetModel
  (QWeakPointer<QcepImageDataBase> img)
  : m_Image(img)
{

}

int QcepImageDataSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  QSharedPointer<QcepImageDataBase> img(m_Image);

  if (img) {
    return img->get_Height();
  } else {
    return 0;
  }
}

int QcepImageDataSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  QSharedPointer<QcepImageDataBase> img(m_Image);

  if (img) {
    return img->get_Width();
  } else {
    return 0;
  }
}

QVariant QcepImageDataSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (!index.isValid()) {
    res = QVariant();
  } else if (role == Qt::DisplayRole) {
    QSharedPointer<QcepImageDataBase> img(m_Image);

    if (img) {
      double v = img->getImageData(index.column(), index.row());
      res = v;
    }
  }

  return res;
}

