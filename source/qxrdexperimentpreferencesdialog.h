#ifndef QXRDEXPERIMENTPREFERENCESDIALOG_H
#define QXRDEXPERIMENTPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QCheckBox>
#include "ui_qxrdexperimentpreferencesdialog.h"

#include "qxrdexperiment.h"

class QxrdExperimentPreferencesDialog : public QDialog, public Ui::QxrdExperimentPreferencesDialog {
  Q_OBJECT
public:
  QxrdExperimentPreferencesDialog(QxrdExperimentWPtr exptw, QWidget *parent = 0);
  ~QxrdExperimentPreferencesDialog();

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
  QxrdExperimentWPtr m_Experiment;
};

#endif // QXRDEXPERIMENTPREFERENCESDIALOG_H
