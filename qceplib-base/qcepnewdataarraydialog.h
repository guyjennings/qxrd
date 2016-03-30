#ifndef QCEPNEWDATAARRAYDIALOG_H
#define QCEPNEWDATAARRAYDIALOG_H

#include <QDialog>
#include "ui_qcepnewdataarraydialog.h"

#include "qcepdatasetmodel-ptr.h"

class QcepNewDataArrayDialog : public QDialog, public Ui::QcepNewDataArrayDialog
{
  Q_OBJECT

public:
  explicit QcepNewDataArrayDialog(QcepDatasetModelPtr model, const QModelIndex &idx);
  ~QcepNewDataArrayDialog();

protected:
  void accept();

private:
  QcepDatasetModelPtr m_Model;
  QModelIndex         m_Index;
};

#endif // QCEPNEWDATAARRAYDIALOG_H
