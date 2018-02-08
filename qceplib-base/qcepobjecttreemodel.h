#ifndef QCEPOBJECTTREEMODEL_H
#define QCEPOBJECTTREEMODEL_H

#include "qceplib_global.h"
#include <QAbstractItemModel>
#include "qcepobject-ptr.h"

class QCEP_EXPORT QcepObjectTreeModel : public QAbstractItemModel
{
  Q_OBJECT

private:
  typedef QAbstractItemModel inherited;

public:
  QcepObjectTreeModel(QObject *parent, QcepObjectWPtr obj);

  enum {
    NameColumn,
    ClassColumn,
    AddressColumn,
    ColumnCount
  };

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QcepObjectPtr indexedObject(const QModelIndex& index) const;

private:
  QcepObjectWPtr m_Object;
};

#endif // QCEPOBJECTTREEMODEL_H
