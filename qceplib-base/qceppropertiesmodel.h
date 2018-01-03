#ifndef QCEPPROPERTIESMODEL_H
#define QCEPPROPERTIESMODEL_H

#include "qceplib_global.h"
#include <QAbstractTableModel>

class QCEP_EXPORT QcepPropertiesModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepPropertiesModel();

public slots:
  void onDataObjectChanged();
};

#endif // QCEPPROPERTIESMODEL_H
