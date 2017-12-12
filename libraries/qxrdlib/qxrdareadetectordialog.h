#ifndef QXRDAREADETECTORDIALOG_H
#define QXRDAREADETECTORDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include <QDialog>

namespace Ui {
class QxrdAreaDetectorDialog;
}

class QxrdAreaDetectorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdAreaDetectorDialog(QWidget *parent = 0);
  ~QxrdAreaDetectorDialog();

private:
  Ui::QxrdAreaDetectorDialog *ui;
};

#endif // QXRDAREADETECTORDIALOG_H
