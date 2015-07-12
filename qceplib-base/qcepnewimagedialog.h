#ifndef QCEPNEWIMAGEDIALOG_H
#define QCEPNEWIMAGEDIALOG_H

#include <QDialog>
#include "ui_qcepnewimagedialog.h"

class QcepDataObject;

class QcepNewImageDialog : public QDialog, public Ui::QcepNewImageDialog
{
  Q_OBJECT

public:
  explicit QcepNewImageDialog(QcepDataObject *obj, QWidget *parent = 0);
  ~QcepNewImageDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDataObject *m_Object;
};

#endif // QCEPNEWIMAGEDIALOG_H
