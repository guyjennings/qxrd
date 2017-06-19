#ifndef QCEPDATASETSELECTIONDIALOG_H
#define QCEPDATASETSELECTIONDIALOG_H

#include <QDialog>
#include "ui_qcepdatasetselectiondialog.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdataobject-ptr.h"

class QcepDatasetSelectionDialog : public QDialog, public Ui::QcepDatasetSelectionDialog
{
  Q_OBJECT

public:
  explicit QcepDatasetSelectionDialog(QString title, QcepDatasetModelPtr ds, const QModelIndex &idx);
  ~QcepDatasetSelectionDialog();

  void accept();

  bool constantChosen();
  double constantValue();
  QModelIndexList selectedIndexes();

private:
  QString             m_Title;
  QcepDatasetModelPtr m_DatasetModel;
  QcepDataObjectPtr   m_Destination;
  QModelIndexList     m_SelectedData;

public:
  static double       m_ConstantValue;
  static int          m_ConstantSelected;
};

#endif // QCEPDATASETSELECTIONDIALOG_H
