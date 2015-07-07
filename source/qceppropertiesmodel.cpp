#include "qceppropertiesmodel.h"

QcepPropertiesModel::QcepPropertiesModel()
{

}

void QcepPropertiesModel::onDataObjectChanged()
{
  beginResetModel();
  endResetModel();
}
