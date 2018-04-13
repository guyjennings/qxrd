#ifndef QCEPCALIBRANTLIBRARYMODEL_H
#define QCEPCALIBRANTLIBRARYMODEL_H

#include "qceplib_global.h"
#include <QAbstractTableModel>
#include "qcepcalibrantlibrary-ptr.h"

class QCEP_EXPORT QcepCalibrantLibraryModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepCalibrantLibraryModel(QcepCalibrantLibraryWPtr cal);
  ~QcepCalibrantLibraryModel();

  void setCalibrantLibrary(QcepCalibrantLibraryWPtr cal);

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
  QcepCalibrantLibraryWPtr m_CalibrantLibrary;
};

#endif // QCEPCALIBRANTLIBRARYMODEL_H
