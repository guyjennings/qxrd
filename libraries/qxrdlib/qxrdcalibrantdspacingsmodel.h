#ifndef QXRDCALIBRANTDSPACINGSMODEL_H
#define QXRDCALIBRANTDSPACINGSMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrant.h"
#include "qxrdcalibrantdspacings-ptr.h"

class QXRD_EXPORT QxrdCalibrantDSpacingsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdCalibrantDSpacingsModel(QxrdCalibrantLibraryWPtr cal, QxrdCalibrantDSpacingsWPtr vec);
  ~QxrdCalibrantDSpacingsModel();

  void setCalibrantDSpacings(QxrdCalibrantDSpacingsWPtr dsp);

  virtual QVariant	   data ( const QModelIndex & index, int role ) const;
  virtual QVariant         headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  virtual int	           rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual int	           columnCount ( const QModelIndex & parent = QModelIndex() ) const;

  void everythingChanged(int nrows);

  enum {
    IndexColumn,
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
  QxrdCalibrantLibraryWPtr   m_CalibrantLibrary;
  QxrdCalibrantDSpacingsWPtr m_CalibrantDSpacings;
};

#endif // QXRDCALIBRANTDSPACINGSMODEL_H
