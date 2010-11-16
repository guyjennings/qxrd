#ifndef QXRDPREFERENCESDIALOG_H
#define QXRDPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>
#include <QLineEdit>

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

private slots:
  void currentOutputBrowse();
  void currentLogfileBrowse();
  void saveRawBrowse();
  void saveDarkBrowse();
  void saveSubtractedBrowse();
  void saveIntegratedBrowse();

protected:
  void changeEvent(QEvent *e);

private:
  void getRelativeDirectoryPath(QLineEdit *edit);

private:
  Ui::QxrdPreferencesDialog *ui;
};

#endif // QXRDPREFERENCESDIALOG_H
