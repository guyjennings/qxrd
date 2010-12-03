#ifndef QXRDMASKLISTMODEL_H
#define QXRDMASKLISTMODEL_H

#include <QAbstractListModel>
#include "qxrdmaskstack.h"

class QxrdMaskListModel : public QAbstractListModel
{
  Q_OBJECT;

public:
  QxrdMaskListModel(QxrdMaskStackPtr masks);

  QxrdMaskStackPtr maskStack();
  void setMaskStack(QxrdMaskStackPtr masks);

  virtual QVariant	     data ( const QModelIndex & index, int role ) const;
  virtual Qt::ItemFlags    flags ( const QModelIndex & index ) const;
  virtual bool             insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
  virtual bool             removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
  virtual int	             rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual bool             setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
  //    virtual void             sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
  virtual Qt::DropActions  supportedDropActions () const;

public slots:
  void onMaskChanged();

private:
  QxrdMaskStackPtr m_MaskStack;
};

typedef QxrdMaskListModel *QxrdMaskListModelPtr;

#endif // QXRDMASKLISTMODEL_H
