#ifndef QXRDPREFERENCESDIALOG_H
#define QXRDPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QCheckBox>
#include "ui_qxrdpreferencesdialog.h"

class QxrdPreferencesDialog : public QDialog, public Ui::QxrdPreferencesDialog {
  Q_OBJECT
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
  void setupDebugWidgets(int dbg);
  int readDebugWidgets();

private:
  QVector<QCheckBox*> m_DebugWidgetList;
};

#endif // QXRDPREFERENCESDIALOG_H
