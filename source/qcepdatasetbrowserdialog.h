#ifndef QCEPDATASETBROWSERDIALOG_H
#define QCEPDATASETBROWSERDIALOG_H

#include <QWidget>

namespace Ui {
  class QcepDatasetBrowserDialog;
}

class QcepDatasetBrowserDialog : public QWidget
{
  Q_OBJECT

public:
  explicit QcepDatasetBrowserDialog(QWidget *parent = 0);
  ~QcepDatasetBrowserDialog();

private:
  Ui::QcepDatasetBrowserDialog *ui;
};

#endif // QCEPDATASETBROWSERDIALOG_H
