#ifndef QCEPNEWDATAGROUPDIALOG_H
#define QCEPNEWDATAGROUPDIALOG_H

#include <QDialog>
#include "ui_qcepnewdatagroupdialog.h"

#include "qcepdatasetmodel-ptr.h"

class QcepNewDataGroupDialog : public QDialog, public Ui::QcepNewDataGroupDialog
{
  Q_OBJECT

public:
  explicit QcepNewDataGroupDialog(QcepDatasetModelPtr model, const QModelIndex &idx);
  ~QcepNewDataGroupDialog();

  void accept();

private:
  QcepDatasetModelPtr m_Model;
  QModelIndex         m_Index;
};

#endif // QCEPNEWDATAGROUPDIALOG_H
