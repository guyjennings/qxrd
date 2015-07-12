#include "qcepspreadsheetmodel.h"
#include <stdio.h>

QcepSpreadsheetModel::QcepSpreadsheetModel()
{

}

void QcepSpreadsheetModel::onDataObjectChanged()
{
  printf("QcepSpreadsheetModel::onDataObjectChanged\n");

  beginResetModel();
  endResetModel();
}

