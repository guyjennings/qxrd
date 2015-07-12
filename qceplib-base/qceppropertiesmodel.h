#ifndef QCEPPROPERTIESMODEL_H
#define QCEPPROPERTIESMODEL_H

#include <QAbstractTableModel>

class QcepPropertiesModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepPropertiesModel();

private slots:
  void onDataObjectChanged();
};

#endif // QCEPPROPERTIESMODEL_H
