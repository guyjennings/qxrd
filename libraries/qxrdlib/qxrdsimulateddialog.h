#ifndef QXRDSIMULATEDDIALOG_H
#define QXRDSIMULATEDDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include <QDialog>

namespace Ui {
class QxrdSimulatedDialog;
}

class QxrdSimulatedDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdSimulatedDialog(QWidget *parent = 0);
  ~QxrdSimulatedDialog();

private:
  Ui::QxrdSimulatedDialog *ui;
};

#endif // QXRDSIMULATEDDIALOG_H
