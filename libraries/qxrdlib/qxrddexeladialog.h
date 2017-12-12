#ifndef QXRDDEXELADIALOG_H
#define QXRDDEXELADIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include <QDialog>

namespace Ui {
class QxrdDexelaDialog;
}

class QxrdDexelaDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdDexelaDialog(QWidget *parent = 0);
  ~QxrdDexelaDialog();

private:
  Ui::QxrdDexelaDialog *ui;
};

#endif // QXRDDEXELADIALOG_H
