#include "qxrdcalibrantlibrarymodel.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrant.h"

QxrdCalibrantLibraryModel::QxrdCalibrantLibraryModel(QxrdCalibrantLibraryPtr cal)
  : m_CalibrantLibrary(cal)
{

}

QxrdCalibrantLibraryModel::~QxrdCalibrantLibraryModel()
{

}

int QxrdCalibrantLibraryModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return m_CalibrantLibrary->count();
}

int QxrdCalibrantLibraryModel::columnCount(const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return NumColumns;
}

QVariant QxrdCalibrantLibraryModel::data (const QModelIndex & index, int role) const
{
  if (index.row() < 0 || index.row() >= m_CalibrantLibrary->count()) {
    return QVariant();
  }

  QxrdCalibrantPtr cal = m_CalibrantLibrary->calibrant(index.row());

  if (cal) {
    int col = index.column();

    if (col == FlagsColumn) {
      if (role == Qt::DecorationRole) {
        if (cal->isLocked()) {
          return QPixmap(":/images/lock-16x16.png");
        }
//      } else if (role == Qt::SizeHintRole) {
//        return QSize(50,20);
      }
    } else if (col == NameColumn) {
      if (role == Qt::DisplayRole) {
        return cal->get_Name();
      }
    } else if (col == SymmetryColumn) {
      if (role == Qt::DisplayRole) {
        switch (cal->get_Symmetry()) {
        case QxrdCalibrant::Hexagonal:
          return "Hexagonal";
          break;

        case QxrdCalibrant::RHexagonal:
          return "R-Hexagonal";
          break;

        case QxrdCalibrant::SimpleCubic:
          return "Cubic";
          break;

        case QxrdCalibrant::BodyCenteredCubic:
          return "BCC";
          break;

        case QxrdCalibrant::FaceCenteredCubic:
          return "FCC";
          break;

        case QxrdCalibrant::DiamondCubic:
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

  return QVariant();
}

QVariant QxrdCalibrantLibraryModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (columnCount() == 1) {
    return QVariant();
  } else {
    if (orientation==Qt::Horizontal) {
      if (role==Qt::DisplayRole) {
        switch (section) {
        case FlagsColumn:
          return "F";
          break;

        case SymmetryColumn:
          return "Symm";
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
