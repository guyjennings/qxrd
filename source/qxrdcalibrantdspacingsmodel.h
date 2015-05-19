#ifndef QXRDCALIBRANTDSPACINGSMODEL_H
#define QXRDCALIBRANTDSPACINGSMODEL_H

#include <QAbstractTableModel>
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrant.h"

class QxrdCalibrantDSpacingsModel : public QAbstractTableModel
{
public:
  QxrdCalibrantDSpacingsModel(QxrdCalibrantLibraryPtr cal, QxrdCalibrantDSpacingVector *vec);
  ~QxrdCalibrantDSpacingsModel();

  virtual QVariant	   data ( const QModelIndex & index, int role ) const;
  virtual QVariant         headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  virtual int	           rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual int	           columnCount ( const QModelIndex & parent = QModelIndex() ) const;

  void everythingChanged(int nrows);

  enum {
    FlagsColumn,
    HColumn,
    KColumn,
    LColumn,
    NColumn,
    DColumn,
    TTHColumn,
    NumColumns
  };
private:
  QxrdCalibrantLibraryPtr m_CalibrantLibrary;
  QxrdCalibrantDSpacingVector *m_CalibrantDSpacingVector;
};

#endif // QXRDCALIBRANTDSPACINGSMODEL_H
