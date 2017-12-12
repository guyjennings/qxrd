#ifndef QXRDPERKINELMERDIALOG_H
#define QXRDPERKINELMERDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include <QDialog>

namespace Ui {
class QxrdPerkinElmerDialog;
}

class QxrdPerkinElmerDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdPerkinElmerDialog(QWidget *parent = 0);
  ~QxrdPerkinElmerDialog();

private:
  Ui::QxrdPerkinElmerDialog *ui;
};

#endif // QXRDPERKINELMERDIALOG_H
