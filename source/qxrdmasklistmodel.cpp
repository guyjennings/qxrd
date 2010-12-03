#include "qxrdmasklistmodel.h"

QxrdMaskListModel::QxrdMaskListModel(QxrdMaskStackPtr masks) :
    m_MaskStack(masks)
{
  connect(masks, SIGNAL(maskChanged()), this, SLOT(onMaskChanged()));
}

int QxrdMaskListModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return m_MaskStack->count();
}

QVariant	 QxrdMaskListModel::data (const QModelIndex & index, int role) const
{
  if (index.row() < 0 || index.row() >= m_MaskStack->count()) {
    return QVariant();
  }

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    QxrdMaskDataPtr p = m_MaskStack->at(index.row());

    if (p) {
      return tr("%1 : %2").arg(index.row()).arg(p->get_Title());
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdMaskListModel::flags (const QModelIndex & index) const
{
  if (!index.isValid()) {
    return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
  }

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool QxrdMaskListModel::insertRows ( int row, int count, const QModelIndex & parent)
{
}

bool QxrdMaskListModel::removeRows ( int row, int count, const QModelIndex & parent)
{
}

bool QxrdMaskListModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
  if ((index.row() >= 0)
      && (index.row() < m_MaskStack->count())
      && (role == Qt::EditRole || role == Qt::DisplayRole)) {

    QxrdMaskDataPtr p = m_MaskStack->at(index.row());

    if (p) {
      p->set_Title(value.toString());
    }

    emit dataChanged(index, index);

    return true;
  }

  return false;
}

//void QxrdMaskListModel::sort ( int column, Qt::SortOrder order)
//{
//}

Qt::DropActions QxrdMaskListModel::supportedDropActions () const
{
  return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}

QxrdMaskStackPtr QxrdMaskListModel::maskStack()
{
  return m_MaskStack;
}

void QxrdMaskListModel::setMaskStack(QxrdMaskStackPtr masks)
{
  emit beginResetModel();

  m_MaskStack = masks;

  emit endResetModel();
}

void QxrdMaskListModel::onMaskChanged()
{
  emit beginResetModel();

  emit endResetModel();
}
