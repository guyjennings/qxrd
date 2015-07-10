#ifndef QXRDNEWIMAGEDIALOG_H
#define QXRDNEWIMAGEDIALOG_H

#include <QDialog>
#include "ui_qxrdnewimagedialog.h"

class QcepDataObject;

class QxrdNewImageDialog : public QDialog, public Ui::QxrdNewImageDialog
{
  Q_OBJECT

public:
  explicit QxrdNewImageDialog(QcepDataObject *obj, QWidget *parent = 0);
  ~QxrdNewImageDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QcepDataObject *m_Object;
};

#endif // QXRDNEWIMAGEDIALOG_H
