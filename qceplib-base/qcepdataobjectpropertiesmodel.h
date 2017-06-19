#ifndef QCEPDATAOBJECTPROPERTIESMODEL_H
#define QCEPDATAOBJECTPROPERTIESMODEL_H

#include "qcepdataobject-ptr.h"
#include "qceppropertiesmodel.h"

class QcepDataObjectPropertiesModel : public QcepPropertiesModel
{
public:
  QcepDataObjectPropertiesModel(QcepDataObjectWPtr obj);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  int propertyCount() const;
  int dynamicPropertyCount() const;

private:
  QcepDataObjectWPtr m_Object;
};

#endif // QCEPDATAOBJECTPROPERTIESMODEL_H
