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

int QxrdMaskListModel::columnCount(const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return 1;
}

QVariant	 QxrdMaskListModel::data (const QModelIndex & index, int role) const
{
  if (index.row() < 0 || index.row() >= m_MaskStack->count()) {
    return QVariant();
  }

  QxrdMaskDataPtr p = m_MaskStack->at(index.row());

  if (p) {
    int col = index.column();

//    if (col == ThumbnailColumn) {
//      if (role == Qt::DecorationRole) {
//        return p->thumbnailImage();
//      }
//    } else if (col == VisibilityColumn) {
//      if (role == Qt::CheckStateRole) {
//        return p->get_Used();
//      }
//    } else if (col == TitleColumn) {
    if (col == 0) {
      if (role == Qt::DecorationRole) {
        return p->thumbnailImage();
      } else if (role == Qt::CheckStateRole) {
        return p->get_Used();
      } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
//        return tr("%1 : %2").arg(m_MaskStack->stackLevelName(index.row())).arg(p->get_Title());
        return p->get_Title();
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdMaskListModel::flags (const QModelIndex & index) const
{
  if (!index.isValid()) {
    return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
  }

  return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable |Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool QxrdMaskListModel::insertRows ( int row, int count, const QModelIndex & parent)
{
}

bool QxrdMaskListModel::removeRows ( int row, int count, const QModelIndex & parent)
{
}

bool QxrdMaskListModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
  if (index.column() == 0) {
    if ((index.row() >= 0) && (index.row() < m_MaskStack->count())) {
      QxrdMaskDataPtr p = m_MaskStack->at(index.row());

      if ((role == Qt::EditRole || role == Qt::DisplayRole)) {
        if (p) {
          p->set_Title(value.toString());
        }

        emit dataChanged(index, index);

        return true;
      }
    }
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
