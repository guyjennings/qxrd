#ifndef QCEPSPREADSHEETMODEL_H
#define QCEPSPREADSHEETMODEL_H

#include <QAbstractTableModel>

class QcepSpreadsheetModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  QcepSpreadsheetModel();

private slots:
  void onDataObjectChanged();
};

#endif // QCEPSPREADSHEETMODEL_H
