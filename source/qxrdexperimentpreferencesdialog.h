#ifndef QXRDEXPERIMENTPREFERENCESDIALOG_H
#define QXRDEXPERIMENTPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QCheckBox>
#include "ui_qxrdexperimentpreferencesdialog.h"

#include "qxrdexperiment-ptr.h"

class QxrdExperimentPreferencesDialog : public QDialog, public Ui::QxrdExperimentPreferencesDialog {
  Q_OBJECT
public:
  QxrdExperimentPreferencesDialog(QxrdExperimentWPtr exptw, QWidget *parent = 0, int initialPage = -1);
  ~QxrdExperimentPreferencesDialog();

public slots:
  void accept();

private slots:
  void dataDirectoryBrowse();
  void currentLogFileBrowse();
  void integratedScansFileBrowse();
  void saveRawBrowse();
  void saveDarkBrowse();
  void saveSubtractedBrowse();
  void saveIntegratedBrowse();

protected:
  void changeEvent(QEvent *e);

private:
  void getRelativeDirectoryPath(QLineEdit *edit);

  QString experimentDirectory();
  QString dataDirectory();

private:
  QxrdExperimentWPtr m_Experiment;
};

#endif // QXRDEXPERIMENTPREFERENCESDIALOG_H
