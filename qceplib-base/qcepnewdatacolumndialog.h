#ifndef QCEPNEWDATACOLUMNDIALOG_H
#define QCEPNEWDATACOLUMNDIALOG_H

#include <QDialog>
#include "ui_qcepnewdatacolumndialog.h"

class QcepDataObject;

class QcepNewDataColumnDialog : public QDialog, public Ui::QcepNewDataColumnDialog
{
  Q_OBJECT

public:
  explicit QcepNewDataColumnDialog(QcepDataObject *obj, QWidget *parent = 0);
  ~QcepNewDataColumnDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDataObject *m_Object;
};

#endif // QCEPNEWDATACOLUMNDIALOG_H
