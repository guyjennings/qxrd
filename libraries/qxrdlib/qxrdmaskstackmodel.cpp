#include "qxrdmaskstackmodel.h"
#include "qxrdmaskstack.h"
#include "qcepmaskdata.h"

QxrdMaskStackModel::QxrdMaskStackModel(QxrdMaskStackWPtr masks) :
    m_MaskStack(masks)
{
  connect(masks.data(), &QxrdMaskStack::maskChanged,
          this,         &QxrdMaskStackModel::onMaskChanged);
}

int QxrdMaskStackModel::rowCount (const QModelIndex & parent) const
{
  int res = 0;

  if (!parent.isValid()) {
    QxrdMaskStackPtr m(m_MaskStack);

    if (m) {
      res = m->maskCount();
    }
  }

  return res;
}

int QxrdMaskStackModel::columnCount(const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return NumColumns;
}

static int thumbnailCount = 0;

QVariant QxrdMaskStackModel::data (const QModelIndex & index, int role) const
{
  if (index.row() < 0 || index.row() >= rowCount()) {
    return QVariant();
  }

  QxrdMaskStackPtr m(m_MaskStack);

  if (m) {
    QcepMaskDataPtr p = m->mask(index.row());

    if (p) {
      int col = index.column();

      if (columnCount()==1) {
        if (col == 0) {
          if (role == Qt::DecorationRole) {
//            printf("%d: Get mask thumbnail\n", thumbnailCount++);
            //TODO: implement thumbnail image cacheing...
            return p->thumbnailImage();
          } else if (role == Qt::CheckStateRole) {
            return (p->get_Used()?Qt::Checked:Qt::Unchecked);
          } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
            //        return tr("%1 : %2").arg(m_MaskStack->stackLevelName(index.row())).arg(p->get_Name());
            return p->get_Name();
          }
        }
      } else {
        if (col == ThumbnailColumn) {
          if (role == Qt::DecorationRole) {
//            printf("%d: Get mask thumbnail\n", thumbnailCount++);
            //TODO: implement thumbnail image cacheing...
            return p->thumbnailImage();
          } else if (role == Qt::SizeHintRole) {
            return p->thumbnailImageSize();
          }
        } else if (col == VisibilityColumn) {
          if (role == Qt::CheckStateRole) {
            return (p->get_Used()?Qt::Checked:Qt::Unchecked);
          } else if (role == Qt::SizeHintRole) {
            return 30;
          }
        } else if (col == TitleColumn) {
          if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return p->get_Name();
          } else if (role==Qt::SizeHintRole) {
            return 120;
          }
        }
      }
    }
  }

  return QVariant();
}

QVariant QxrdMaskStackModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (columnCount() == 1) {
    return QVariant();
  } else {
    if (orientation==Qt::Horizontal && role==Qt::DisplayRole) {
      switch (section) {
      case VisibilityColumn:
        return "Vis";

      case ThumbnailColumn:
        return "";

      case TitleColumn:
        return "Name";
      }
    } else if (orientation==Qt::Vertical && role==Qt::DisplayRole) {
      return QxrdMaskStack::stackLevelName(section);
    }
  }

  return QVariant();
}

Qt::ItemFlags QxrdMaskStackModel::flags (const QModelIndex & index) const
{
  if (!index.isValid()) {
    return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
  }

  return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable |Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable;
}

bool QxrdMaskStackModel::insertRows ( int row, int count, const QModelIndex & /*parent*/)
{
  printf("QxrdMaskStackModel::insertRows(%d,%d,...)\n", row, count);

  return false;
}

bool QxrdMaskStackModel::removeRows ( int row, int count, const QModelIndex & /*parent*/)
{
  printf("QxrdMaskStackModel::removeRows(%d,%d,...)\n", row, count);

  return false;
}

bool QxrdMaskStackModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
  if (columnCount() == 1) {
    if (index.column() == 0) {
      if ((index.row() >= 0) && (index.row() < rowCount())) {
        QxrdMaskStackPtr m(m_MaskStack);

        if (m) {
          QcepMaskDataPtr p = m->mask(index.row());

          if ((role == Qt::EditRole || role == Qt::DisplayRole)) {
            if (p) {
              p->set_Name(value.toString());
            }

            emit dataChanged(index, index);

            return true;
          }

          if (role == Qt::CheckStateRole) {
            if (p) {
              p->set_Used(!(p->get_Used()));
            }

            emit dataChanged(index, index);

            return true;
          }
        }
      }
    }
  } else {
    if ((index.row() >= 0) && (index.row() < rowCount())) {
      QxrdMaskStackPtr m(m_MaskStack);

      if (m) {
        QcepMaskDataPtr p = m->mask(index.row());

        if (index.column() == TitleColumn && (role == Qt::EditRole || role == Qt::DisplayRole)) {
          if (p) {
            p->set_Name(value.toString());
          }

          emit dataChanged(index, index);

          return true;
        }

        if (index.column() == VisibilityColumn && (role == Qt::CheckStateRole)) {
          if (p) {
            p->set_Used(!(p->get_Used()));
          }

          emit dataChanged(index, index);

          return true;
        }
      }
    }
  }

  return false;
}

//void QxrdMaskStackModel::sort ( int column, Qt::SortOrder order)
//{
//}

Qt::DropActions QxrdMaskStackModel::supportedDropActions () const
{
  return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}

QxrdMaskStackWPtr QxrdMaskStackModel::maskStack()
{
  return m_MaskStack;
}

void QxrdMaskStackModel::setMaskStack(QxrdMaskStackWPtr masks)
{
  emit beginResetModel();

  m_MaskStack = masks;

  emit endResetModel();
}

void QxrdMaskStackModel::onMaskChanged()
{
  emit beginResetModel();

  emit endResetModel();
}