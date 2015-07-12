#include "qcepimagedataspreadsheetmodel.h"
#include <QModelIndex>

QcepImageDataSpreadsheetModel::QcepImageDataSpreadsheetModel
  (QSharedPointer<QcepImageDataBase> img)
  : m_Image(img)
{

}

int QcepImageDataSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  return m_Image->get_Height();
}

int QcepImageDataSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return m_Image->get_Width();
}

QVariant QcepImageDataSpreadsheetModel::data(const QModelIndex &index, int role) const
{
  QVariant res = QVariant();

  if (!index.isValid()) {
    res = QVariant();
  } else if (role == Qt::DisplayRole) {
    double v = m_Image->getImageData(index.column(), index.row());
    res = v;
  }

  return res;
}

