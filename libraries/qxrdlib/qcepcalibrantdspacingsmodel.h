#ifndef QCEPCALIBRANTDSPACINGSMODEL_H
#define QCEPCALIBRANTDSPACINGSMODEL_H

#include "qceplib_global.h"
#include <QAbstractTableModel>
#include "qcepcalibrantlibrary-ptr.h"
#include "qcepcalibrant.h"
#include "qcepcalibrantdspacings-ptr.h"

class QCEP_EXPORT QcepCalibrantDSpacingsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepCalibrantDSpacingsModel(QcepCalibrantLibraryWPtr cal, QcepCalibrantDSpacingsWPtr vec);
  ~QcepCalibrantDSpacingsModel();

  void setCalibrantDSpacings(QcepCalibrantDSpacingsWPtr dsp);

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
  QcepCalibrantLibraryWPtr   m_CalibrantLibrary;
  QcepCalibrantDSpacingsWPtr m_CalibrantDSpacings;
};

#endif // QCEPCALIBRANTDSPACINGSMODEL_H
