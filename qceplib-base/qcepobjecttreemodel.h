#ifndef QCEPOBJECTTREEMODEL_H
#define QCEPOBJECTTREEMODEL_H

#include "qceplib_global.h"
#include <QAbstractItemModel>
#include "qcepobject-ptr.h"

class QCEP_EXPORT QcepObjectTreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit QcepObjectTreeModel(QObject *parent, QcepObjectWPtr obj);

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
  QcepObjectWPtr m_Object;
};

#endif // QCEPOBJECTTREEMODEL_H
