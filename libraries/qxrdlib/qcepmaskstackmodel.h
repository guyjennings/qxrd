#ifndef QCEPMASKSTACKMODEL_H
#define QCEPMASKSTACKMODEL_H

#include "qceplib_global.h"
#include <QAbstractListModel>
#include "qcepmaskstack-ptr.h"

class QCEP_EXPORT QcepMaskStackModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepMaskStackModel(QcepMaskStackWPtr masks);

  QcepMaskStackWPtr maskStack();
  void setMaskStack(QcepMaskStackWPtr masks);

  virtual QVariant	   data ( const QModelIndex & index, int role ) const;
  virtual QVariant         headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
  virtual Qt::ItemFlags    flags ( const QModelIndex & index ) const;
  virtual bool             insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
  virtual bool             removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
  virtual int	           rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual int	           columnCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual bool             setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
  //    virtual void             sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
  virtual Qt::DropActions  supportedDropActions () const;
  enum {
    VisibilityColumn,
    ThumbnailColumn,
    TitleColumn,
    NumColumns
  };

public slots:
  void onMaskChanged();

private:
  QcepMaskStackWPtr m_MaskStack;
};

#endif // QCEPMASKSTACKMODEL_H
