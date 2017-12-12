#ifndef QXRDPILATUSDIALOG_H
#define QXRDPILATUSDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include <QDialog>

namespace Ui {
class QxrdPilatusDialog;
}

class QxrdPilatusDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdPilatusDialog(QWidget *parent = 0);
  ~QxrdPilatusDialog();

private:
  Ui::QxrdPilatusDialog *ui;
};

#endif // QXRDPILATUSDIALOG_H
