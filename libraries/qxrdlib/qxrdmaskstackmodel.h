#ifndef QXRDMASKSTACKMODEL_H
#define QXRDMASKSTACKMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractListModel>
#include "qxrdmaskstack-ptr.h"

class QXRD_EXPORT QxrdMaskStackModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QxrdMaskStackModel(QxrdMaskStackWPtr masks);

  QxrdMaskStackWPtr maskStack();
  void setMaskStack(QxrdMaskStackWPtr masks);

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
  QxrdMaskStackWPtr m_MaskStack;
};

#endif // QXRDMASKSTACKMODEL_H
