#ifndef QCEPNEWCOLUMNSCANDIALOG_H
#define QCEPNEWCOLUMNSCANDIALOG_H

#include <QDialog>
#include "ui_qcepnewcolumnscandialog.h"

class QcepDataObject;

class QcepNewColumnScanDialog : public QDialog, public Ui::QcepNewColumnScanDialog
{
  Q_OBJECT

public:
  explicit QcepNewColumnScanDialog(QcepDataObject *obj, QWidget *parent = 0);
  ~QcepNewColumnScanDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDataObject *m_Object;
};

#endif // QCEPNEWCOLUMNSCANDIALOG_H
