#ifndef QXRDEXPERIMENTPREFERENCESDIALOG_H
#define QXRDEXPERIMENTPREFERENCESDIALOG_H

#include "qcepmacros.h"

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QCheckBox>
#include "ui_qxrdexperimentpreferencesdialog.h"

#include "qxrdexperiment-ptr.h"
#include "qxrddetectorproxy-ptr.h"
#include "qxrddetectorproxylistmodel-ptr.h"

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

  void addDetector();
  void removeDetector();
  void moveDetectorDown();
  void moveDetectorUp();
  void configureDetector();
  void detectorDoubleClicked(const QModelIndex& item);

protected:
  void changeEvent(QEvent *e);

private:
  void getRelativeDirectoryPath(QLineEdit *edit);

  QString experimentDirectory();
  QString dataDirectory();

  void appendDetectorProxy(QxrdDetectorProxyPtr proxy);

private:
  QxrdExperimentWPtr            m_Experiment;
  QxrdDetectorProxyListModelPtr m_DetectorProxyModel;
};

#endif // QXRDEXPERIMENTPREFERENCESDIALOG_H
