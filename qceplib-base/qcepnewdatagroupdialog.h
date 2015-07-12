#ifndef QCEPNEWDATAGROUPDIALOG_H
#define QCEPNEWDATAGROUPDIALOG_H

#include <QDialog>
#include "ui_qcepnewdatagroupdialog.h"

class QcepDataObject;

class QcepNewDataGroupDialog : public QDialog, public Ui::QcepNewDataGroupDialog
{
  Q_OBJECT

public:
  explicit QcepNewDataGroupDialog(QcepDataObject *obj, QWidget *parent = 0);
  ~QcepNewDataGroupDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDataObject *m_Object;
};

#endif // QCEPNEWDATAGROUPDIALOG_H
