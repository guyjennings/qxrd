#ifndef QXRDFILEWATCHERDIALOG_H
#define QXRDFILEWATCHERDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include <QDialog>

namespace Ui {
class QxrdFileWatcherDialog;
}

class QxrdFileWatcherDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QxrdFileWatcherDialog(QWidget *parent = 0);
  ~QxrdFileWatcherDialog();

private:
  Ui::QxrdFileWatcherDialog *ui;
};

#endif // QXRDFILEWATCHERDIALOG_H
