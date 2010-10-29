#ifndef QXRDPREFERENCESDIALOG_H
#define QXRDPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>

namespace Ui {
  class QxrdPreferencesDialog;
}

class QxrdPreferencesDialog : public QDialog {
  Q_OBJECT;
public:
  QxrdPreferencesDialog(QWidget *parent = 0);
  ~QxrdPreferencesDialog();

public slots:
  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdPreferencesDialog *ui;
};

#endif // QXRDPREFERENCESDIALOG_H
