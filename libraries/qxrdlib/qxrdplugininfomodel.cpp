#include "qxrdplugininfomodel.h"
#include <QIcon>

QxrdPluginInfoModel::QxrdPluginInfoModel()
{

}

int QxrdPluginInfoModel::rowCount(const QModelIndex &parent) const
{
  return m_Paths.count();
}

int QxrdPluginInfoModel::columnCount(const QModelIndex &parent) const
{
  return 5;
}

QVariant QxrdPluginInfoModel::data(const QModelIndex &index, int role) const
{
  int r = index.row();
  int c = index.column();

  if (role == Qt::DisplayRole) {
    switch (c) {
    case 0:
      return r;
      break;

    case 1:
      return m_Paths.value(r);
      break;

    case 2:
      return m_ClassNames.value(r);
      break;

//    case 4:
//      break;

    case 4:
      return tr("0x%1").arg(m_Addresses.value(r),2*sizeof(void*), 16, QChar('0'));
      break;
    }
  }

  if (role == Qt::DecorationRole) {
    if (c == 3) {
      if (m_Loaded.value(r)) {
        return QIcon(":/images/button_ok.png");
      }
    }
  }
  return QVariant();
}

QVariant QxrdPluginInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
      case 0:
        return "#";
        break;

      case 1:
        return "File";
        break;

      case 2:
        return "Class";
        break;

      case 3:
        return "LD?";
        break;

      case 4:
        return "Address";
        break;
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdPluginInfoModel::flags(const QModelIndex &index) const
{
  return QAbstractTableModel::flags(index);
}

void QxrdPluginInfoModel::appendEntry(QString path,
                                      QString className,
                                      int     loaded,
                                      quint64 address)
{
  m_Paths.append(path);
  m_ClassNames.append(className);
  m_Loaded.append(loaded);
  m_Addresses.append(address);
}
