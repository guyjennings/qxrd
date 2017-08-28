#ifndef QCEPNEWDATAARRAYDIALOG_H
#define QCEPNEWDATAARRAYDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepnewdataarraydialog.h"

#include "qcepdatasetmodel-ptr.h"

class QCEP_EXPORT QcepNewDataArrayDialog : public QDialog, public Ui::QcepNewDataArrayDialog
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
