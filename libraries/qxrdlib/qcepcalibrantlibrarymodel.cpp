#include "qcepcalibrantlibrarymodel.h"
#include "qcepcalibrantlibrary.h"
#include "qcepcalibrant.h"

QcepCalibrantLibraryModel::QcepCalibrantLibraryModel(QcepCalibrantLibraryWPtr cal)
  : m_CalibrantLibrary(cal)
{

}

QcepCalibrantLibraryModel::~QcepCalibrantLibraryModel()
{

}

void QcepCalibrantLibraryModel::setCalibrantLibrary(QcepCalibrantLibraryWPtr /*cal*/)
{
  printf("Need to write QcepCalibrantLibraryModel::setCalibrantLibrary");
}

int QcepCalibrantLibraryModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);

  if (lib) {
    return lib->count();
  }

  return 0;
}

int QcepCalibrantLibraryModel::columnCount(const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return NumColumns;
}

QVariant QcepCalibrantLibraryModel::data (const QModelIndex & index, int role) const
{
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);

  if (lib) {
    if (index.row() < 0 || index.row() >= lib->count()) {
      return QVariant();
    }

    QcepCalibrantPtr cal = lib->calibrant(index.row());

    if (cal) {
      int col = index.column();

      if (col == IsUsedColumn) {
        if (role == Qt::CheckStateRole) {
          if (cal->get_IsUsed()) {
            return Qt::Checked;
          } else {
            return Qt::Unchecked;
          }
        }
      } else if (col == FlagsColumn) {
        if (role == Qt::DecorationRole) {
          if (cal->isLocked()) {
            return QPixmap(":/images/lock-16x16.png");
          }
        }
      } else if (col == NameColumn) {
        if (role == Qt::DisplayRole) {
          return cal->get_Name();
        }
      } else if (col == SymmetryColumn) {
        if (role == Qt::DisplayRole) {
          switch (cal->get_Symmetry()) {
          case QcepCalibrant::Hexagonal:
            return "Hexagonal";
            break;

          case QcepCalibrant::RHexagonal:
            return "R-Hexagonal";
            break;

          case QcepCalibrant::SimpleCubic:
            return "Cubic";
            break;

          case QcepCalibrant::BodyCenteredCubic:
            return "BCC";
            break;

          case QcepCalibrant::FaceCenteredCubic:
            return "FCC";
            break;

          case QcepCalibrant::DiamondCubic:
            return "Diamond";
            break;
          }
        }
      } else if (col == DescriptionColumn) {
        if (role == Qt::DisplayRole) {
          return cal->get_Description();
          //      } else if (role==Qt::SizeHintRole) {
          //        return QSize(120,20);
        }
      }
    }
  }

  return QVariant();
}

QVariant QcepCalibrantLibraryModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (columnCount() == 1) {
    return QVariant();
  } else {
    if (orientation==Qt::Horizontal) {
      if (role==Qt::DisplayRole) {
        switch (section) {
        case IsUsedColumn:
          return "Used?";
          break;

        case FlagsColumn:
          return "Flags";
          break;

        case SymmetryColumn:
          return "Symmetry";
          break;

        case NameColumn:
          return "Name";
          break;

        case DescriptionColumn:
          return "Description";
          break;
        }
      }
    } else if (orientation==Qt::Vertical) {
      if (role==Qt::DisplayRole) {
        return section;
      }
    }
  }

  return QVariant();
}

int QcepCalibrantLibraryModel::isUsed(int n)
{
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);

  if (lib) {
    QcepCalibrantPtr cal = lib->calibrant(n);

    if (cal) {
      return cal->get_IsUsed();
    }
  }

  return 0;
}

void QcepCalibrantLibraryModel::toggleIsUsed(int n)
{
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);

  if (lib) {
    QcepCalibrantPtr cal = lib->calibrant(n);

    if (cal) {
      cal->set_IsUsed(!cal->get_IsUsed());

      emit dataChanged(index(n,0), index(n,0));
    }
  }
}

void QcepCalibrantLibraryModel::setIsUsed(int n, int v)
{
  QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);

  if (lib) {
    QcepCalibrantPtr cal = lib->calibrant(n);

    if (cal) {
      cal->set_IsUsed(v);

      emit dataChanged(index(n,0), index(n,0));
    }
  }
}

void QcepCalibrantLibraryModel::calibrantChanged(int n)
{
  int nc = columnCount();

  emit dataChanged(index(n,0), index(n,nc-1));
}
