#include "qxrdimagedataspreadsheetmodel.h"
#include <QModelIndex>

QxrdImageDataSpreadsheetModel::QxrdImageDataSpreadsheetModel
  (QSharedPointer<QcepImageDataBase> img)
  : m_Image(img)
{

}

int QxrdImageDataSpreadsheetModel::rowCount(const QModelIndex &parent) const
{
  return m_Image->get_Height();
}

int QxrdImageDataSpreadsheetModel::columnCount(const QModelIndex &parent) const
{
  return m_Image->get_Width();
}

QVariant QxrdImageDataSpreadsheetModel::data(const QModelIndex &index, int role) const
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

