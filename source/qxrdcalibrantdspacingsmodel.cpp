#include "qxrdcalibrantdspacingsmodel.h"
#include "qxrdcalibrantlibrary.h"

QxrdCalibrantDSpacingsModel::QxrdCalibrantDSpacingsModel(QxrdCalibrantLibraryWPtr cal, QxrdCalibrantDSpacingsWPtr vec)
  : m_CalibrantLibrary(cal),
    m_CalibrantDSpacings(vec)
{
}

QxrdCalibrantDSpacingsModel::~QxrdCalibrantDSpacingsModel()
{

}

void QxrdCalibrantDSpacingsModel::setCalibrantDSpacings(QxrdCalibrantDSpacingsWPtr dsp)
{
  printf("Need to write QxrdCalibrantDSpacingsModel::setCalibrantDSpacings\n");
}

int QxrdCalibrantDSpacingsModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  QxrdCalibrantDSpacingsPtr spc(m_CalibrantDSpacings);

  if (spc) {
    return spc->count(); /*m_Calibrant->countDSpacings();*/
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

QVariant QxrdCalibrantDSpacingsModel::data (const QModelIndex & index, int role) const
{
  QxrdCalibrantDSpacingsPtr spc(m_CalibrantDSpacings);

  if (spc) {
    if (index.row() < 0 || index.row() >= spc->count()) {
      return QVariant();
    }
    //  if (role == Qt::DisplayRole) {
    //    return "X";
    //  }

    int col = index.column();
    int row = index.row();

    QxrdCalibrantDSpacing dspc(spc->value(row));

    if (role == Qt::DisplayRole) {
      if (dspc.isValid()) {
        if (col == IndexColumn) {
          QString res="";

          QxrdCalibrantLibraryPtr lib(m_CalibrantLibrary);

          if (lib) {
            QxrdCalibrantPtr cal = lib->calibrant(dspc.index());

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

void QxrdCalibrantDSpacingsModel::everythingChanged(int nrows)
{
//  emit dataChanged(QModelIndex(), QModelIndex());
  beginResetModel();
  endResetModel();
}
