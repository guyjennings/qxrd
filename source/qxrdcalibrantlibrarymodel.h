#ifndef QXRDCALIBRANTLIBRARYMODEL_H
#define QXRDCALIBRANTLIBRARYMODEL_H

#include <QAbstractTableModel>
#include "qxrdcalibrantlibrary-ptr.h"

class QxrdCalibrantLibraryModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdCalibrantLibraryModel(QxrdCalibrantLibraryPtr cal);
  ~QxrdCalibrantLibraryModel();

  virtual int	           rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual int	           columnCount ( const QModelIndex & parent = QModelIndex() ) const;

  virtual QVariant	   data ( const QModelIndex & index, int role ) const;
  virtual QVariant         headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  enum {
    FlagsColumn,
    NameColumn,
    SymmetryColumn,
    DescriptionColumn,
    NumColumns
  };
private:
  QxrdCalibrantLibraryPtr m_CalibrantLibrary;
};

#endif // QXRDCALIBRANTLIBRARYMODEL_H