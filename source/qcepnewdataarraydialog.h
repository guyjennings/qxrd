#ifndef QCEPNEWDATAARRAYDIALOG_H
#define QCEPNEWDATAARRAYDIALOG_H

#include <QDialog>
#include "ui_qcepnewdataarraydialog.h"

class QcepDataObject;

class QcepNewDataArrayDialog : public QDialog, public Ui::QcepNewDataArrayDialog
{
  Q_OBJECT

public:
  explicit QcepNewDataArrayDialog(QcepDataObject *obj, QWidget *parent = 0);
  ~QcepNewDataArrayDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDataObject *m_Object;
};

#endif // QCEPNEWDATAARRAYDIALOG_H
