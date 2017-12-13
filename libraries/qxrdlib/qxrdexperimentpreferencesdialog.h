#ifndef QXRDEXPERIMENTPREFERENCESDIALOG_H
#define QXRDEXPERIMENTPREFERENCESDIALOG_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QCheckBox>
#include "ui_qxrdexperimentpreferencesdialog.h"

#include "qxrdexperiment-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrddetectorlistmodel-ptr.h"

//TODO: detector edition dialog doesnt work any more
class QXRD_EXPORT QxrdExperimentPreferencesDialog : public QDialog, public Ui::QxrdExperimentPreferencesDialog {
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

//  void appendDetector(QxrdDetectorSettingsPtr det);

private:
  QxrdExperimentWPtr       m_Experiment;
  QxrdDetectorListModelPtr m_DetectorsModel;
};

#endif // QXRDEXPERIMENTPREFERENCESDIALOG_H
