#ifndef QCEPSPREADSHEETMODEL_H
#define QCEPSPREADSHEETMODEL_H

#include "qceplib_global.h"
#include <QAbstractTableModel>

class QCEP_EXPORT QcepSpreadsheetModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepSpreadsheetModel();

public slots:
  void onDataObjectChanged();
};

#endif // QCEPSPREADSHEETMODEL_H
