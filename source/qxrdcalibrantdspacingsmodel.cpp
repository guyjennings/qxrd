#include "qxrdcalibrantdspacingsmodel.h"
#include "qxrdcalibrantlibrary.h"

QxrdCalibrantDSpacingsModel::QxrdCalibrantDSpacingsModel(QxrdCalibrantLibraryPtr cal, QxrdCalibrantDSpacingVector *vec)
  : m_CalibrantLibrary(cal),
    m_CalibrantDSpacingVector(vec)
{

}

QxrdCalibrantDSpacingsModel::~QxrdCalibrantDSpacingsModel()
{

}

int QxrdCalibrantDSpacingsModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  if (m_CalibrantDSpacingVector) {
    return m_CalibrantDSpacingVector->count(); /*m_Calibrant->countDSpacings();*/
  } else {
    return 0;
  }
}

int QxrdCalibrantDSpacingsModel::columnCount(const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return NumColumns;
}

QVariant QxrdCalibrantDSpacingsModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (columnCount() == 1) {
    return QVariant();
  } else {
    if (orientation==Qt::Horizontal && role==Qt::DisplayRole) {
      switch (section) {
      case FlagsColumn:
        return "F";

      case HColumn:
        return "H";

      case KColumn:
        return "K";

      case LColumn:
        return "L";

      case DColumn:
        return "D (Angst)";

      case TTHColumn:
        return "TTH (Deg)";
      }
    } else if (orientation==Qt::Vertical && role==Qt::DisplayRole) {
      return section;
    }
  }

  return QVariant();
}

QVariant QxrdCalibrantDSpacingsModel::data (const QModelIndex & index, int role) const
{
  if (m_CalibrantDSpacingVector) {
    if (index.row() < 0 || index.row() >= m_CalibrantDSpacingVector->count()) {
      return QVariant();
    }
    //  if (role == Qt::DisplayRole) {
    //    return "X";
    //  }

    int col = index.column();
    int row = index.row();

    QxrdCalibrantDSpacing spc(m_CalibrantDSpacingVector->value(row));

    if (role == Qt::DisplayRole) {
      if (spc.isValid()) {
        if (col == FlagsColumn) {
          return "X";
        }

        if (col == HColumn) {
          return spc.h();
        }

        if (col == KColumn) {
          return spc.k();
        }

        if (col == LColumn) {
          return spc.l();
        }

        if (col == DColumn) {
          return spc.d();
        }

        if (col == TTHColumn) {
          return spc.tth();
        }
      }
    }
  }
//  QxrdMaskDataPtr p = m_MaskStack->at(index.row());

//  if (p) {
//

//    if (columnCount()==1) {
//      if (col == 0) {
//        if (role == Qt::DecorationRole) {
//          return p->thumbnailImage();
//        } else if (role == Qt::CheckStateRole) {
//          return (p->get_Used()?Qt::Checked:Qt::Unchecked);
//        } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
//          //        return tr("%1 : %2").arg(m_MaskStack->stackLevelName(index.row())).arg(p->get_Title());
//          return p->get_Title();
//        }
//      }
//    } else {
//      if (col == ThumbnailColumn) {
//        if (role == Qt::DecorationRole) {
//          return p->thumbnailImage();
//        } else if (role == Qt::SizeHintRole) {
//          return p->thumbnailImageSize();
//        }
//      } else if (col == VisibilityColumn) {
//        if (role == Qt::CheckStateRole) {
//          return (p->get_Used()?Qt::Checked:Qt::Unchecked);
//        } else if (role == Qt::SizeHintRole) {
//          return 30;
//        }
//      } else if (col == TitleColumn) {
//        if (role == Qt::DisplayRole || role == Qt::EditRole) {
//          return p->get_Title();
//        } else if (role==Qt::SizeHintRole) {
//          return 120;
//        }
//      }
//    }
//  }

  return QVariant();
}

void QxrdCalibrantDSpacingsModel::everythingChanged(int nrows)
{
//  emit dataChanged(QModelIndex(), QModelIndex());
  beginResetModel();
  endResetModel();
}
