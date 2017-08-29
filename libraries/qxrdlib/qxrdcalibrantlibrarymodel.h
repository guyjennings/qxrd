#ifndef QXRDCALIBRANTLIBRARYMODEL_H
#define QXRDCALIBRANTLIBRARYMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractTableModel>
#include "qxrdcalibrantlibrary-ptr.h"

class QXRD_EXPORT QxrdCalibrantLibraryModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdCalibrantLibraryModel(QxrdCalibrantLibraryWPtr cal);
  ~QxrdCalibrantLibraryModel();

  void setCalibrantLibrary(QxrdCalibrantLibraryWPtr cal);

  virtual int	           rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual int	           columnCount ( const QModelIndex & parent = QModelIndex() ) const;

  virtual QVariant	   data ( const QModelIndex & index, int role ) const;
  virtual QVariant         headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

  enum {
    IsUsedColumn,
    FlagsColumn,
    NameColumn,
    SymmetryColumn,
    DescriptionColumn,
    NumColumns
  };

  void toggleIsUsed(int n);
  int isUsed(int n);
  void setIsUsed(int n, int v);

  void calibrantChanged(int n);

private:
  QxrdCalibrantLibraryWPtr m_CalibrantLibrary;
};

#endif // QXRDCALIBRANTLIBRARYMODEL_H
