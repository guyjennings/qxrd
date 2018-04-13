#include "qcepcalibrantdspacingsmodel.h"
#include "qcepcalibrantlibrary.h"

QcepCalibrantDSpacingsModel::QcepCalibrantDSpacingsModel(QcepCalibrantLibraryWPtr cal, QcepCalibrantDSpacingsWPtr vec)
  : m_CalibrantLibrary(cal),
    m_CalibrantDSpacings(vec)
{
}

QcepCalibrantDSpacingsModel::~QcepCalibrantDSpacingsModel()
{

}

void QcepCalibrantDSpacingsModel::setCalibrantDSpacings(QcepCalibrantDSpacingsWPtr /*dsp*/)
{
  printf("Need to write QcepCalibrantDSpacingsModel::setCalibrantDSpacings\n");
}

int QcepCalibrantDSpacingsModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  QcepCalibrantDSpacingsPtr spc(m_CalibrantDSpacings);

  if (spc) {
    return spc->count(); /*m_Calibrant->countDSpacings();*/
  } else {
    return 0;
  }
}

int QcepCalibrantDSpacingsModel::columnCount(const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return NumColumns;
}

QVariant QcepCalibrantDSpacingsModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (columnCount() == 1) {
    return QVariant();
  } else {
    if (orientation==Qt::Horizontal && role==Qt::DisplayRole) {
      switch (section) {
      case IndexColumn:
        return "Cal";

      case FlagsColumn:
        return "F";

      case HColumn:
        return "H";

      case KColumn:
        return "K";

      case LColumn:
        return "L";

      case NColumn:
        return "N";

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

QVariant QcepCalibrantDSpacingsModel::data (const QModelIndex & index, int role) const
{
  QcepCalibrantDSpacingsPtr spc(m_CalibrantDSpacings);

  if (spc) {
    if (index.row() < 0 || index.row() >= spc->count()) {
      return QVariant();
    }
    //  if (role == Qt::DisplayRole) {
    //    return "X";
    //  }

    int col = index.column();
    int row = index.row();

    QcepCalibrantDSpacing dspc(spc->value(row));

    if (role == Qt::DisplayRole) {
      if (dspc.isValid()) {
        if (col == IndexColumn) {
          QString res="";

          QcepCalibrantLibraryPtr lib(m_CalibrantLibrary);

          if (lib) {
            QcepCalibrantPtr cal = lib->calibrant(dspc.index());

            if (cal) {
              res = cal->get_Name();
            }
          }

          return res;
        }

        if (col == FlagsColumn) {
          return "X";
        }

        if (col == HColumn) {
          return dspc.h();
        }

        if (col == KColumn) {
          return dspc.k();
        }

        if (col == LColumn) {
          return dspc.l();
        }

        if (col == NColumn) {
          return dspc.n();
        }

        if (col == DColumn) {
          return dspc.d();
        }

        if (col == TTHColumn) {
          return dspc.tth();
        }
      }
    }
  }

  return QVariant();
}

void QcepCalibrantDSpacingsModel::everythingChanged(int /*nrows*/)
{
//  emit dataChanged(QModelIndex(), QModelIndex());
  beginResetModel();
  endResetModel();
}
