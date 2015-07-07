#ifndef QCEPDATAOBJECTPROPERTIESMODEL_H
#define QCEPDATAOBJECTPROPERTIESMODEL_H

#include "qcepdataobject-ptr.h"
#include <QAbstractTableModel>

class QcepDataObjectPropertiesModel : public QAbstractTableModel
{
public:
  QcepDataObjectPropertiesModel(QcepDataObjectPtr obj);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  int propertyCount() const;
  int dynamicPropertyCount() const;

private:
  QcepDataObjectPtr m_Object;
};

#endif // QCEPDATAOBJECTPROPERTIESMODEL_H
