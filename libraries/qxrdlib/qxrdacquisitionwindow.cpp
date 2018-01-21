#include "qxrdacquisitionwindow.h"
#include "qxrdexperiment.h"
#include "qxrdacquisitionwindowsettings.h"
#include "qxrdcorrectiondialog.h"
#include "qxrddetectorlistmodel.h"
#include <QTableView>
#include <QDir>
#include <QFileDialog>

QxrdAcquisitionWindow::QxrdAcquisitionWindow(QxrdAcquisitionWindowSettingsWPtr set,
                                             QString name,
                                             QxrdApplicationWPtr app,
                                             QxrdExperimentWPtr expt,
                                             QxrdAcquisitionWPtr acqw,
                                             QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw),
  m_AcquisitionWindowSettings(set)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);

  m_DatasetBrowserView -> setExperiment(expt);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdDataProcessorPtr proc(exp->dataProcessor());

    QxrdAcquisitionWindowSettingsPtr settings(m_AcquisitionWindowSettings);

    if (settings) {
      m_FileBrowserWidget -> initialize(settings->fileBrowserSettings(), exp, proc);
    }

    m_DetectorsModel =
        QxrdDetectorListModelPtr(new QxrdDetectorListModel());

    m_Acquisition = exp->acquisition();

    QxrdAcquisitionPtr acqp(m_Acquisition);

    if (acqp) {
      for (int i=0; i<acqp->get_DetectorCount(); i++) {
        QxrdDetectorSettingsPtr det = acqp->detector(i);

        m_DetectorsModel -> append(det);
      }
    }

    m_AcquisitionDetectorTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_AcquisitionDetectorTreeView->setModel(m_DetectorsModel.data());

    exp  -> prop_ExperimentDirectory() -> linkTo(this -> m_ExperimentDirectory);
    exp  -> prop_LogFileName() -> linkTo(this -> m_LogFileName);
    exp  -> prop_DataDirectory() -> linkTo(this -> m_DataDirectory);
    exp  -> prop_ScanFileName() -> linkTo(this -> m_ScanFileName);

    connect(m_LogFileName, &QLineEdit::editingFinished, exp.data(), &QxrdExperiment::openNewLogFile);

    if (acqp) {
      connect(m_ActionAcquire,     &QAction::triggered, acqp.data(), &QxrdAcquisition::acquire);
      connect(m_ActionAcquireOnce, &QAction::triggered, acqp.data(), &QxrdAcquisition::acquireOnce);
      connect(m_ActionCancel,      &QAction::triggered, acqp.data(), &QxrdAcquisition::cancel);
      connect(m_ActionAcquireDark, &QAction::triggered, acqp.data(), &QxrdAcquisition::acquireDark);
      connect(m_ActionTrigger,     &QAction::triggered, acqp.data(), &QxrdAcquisition::trigger);

      connect(m_BrowseLogFileButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::browseLogFile);
      connect(m_BrowseScanFileButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::browseScanFile);

      connect(m_AcquireButton, &QAbstractButton::clicked, m_ActionAcquire, &QAction::triggered);
      connect(m_AcquireOnceButton, &QAbstractButton::clicked, m_ActionAcquireOnce, &QAction::triggered);
      connect(m_CancelButton, &QAbstractButton::clicked, m_ActionCancel, &QAction::triggered);
      connect(m_TriggerButton, &QAbstractButton::clicked, m_ActionTrigger, &QAction::triggered);
      connect(m_DarkAcquireButton, &QAbstractButton::clicked, m_ActionAcquireDark, &QAction::triggered);

      connect(m_ClearDroppedButton, &QAbstractButton::clicked, acqp.data(), &QxrdAcquisition::clearDropped);

      connect(acqp.data(), &QxrdAcquisition::acquireStarted, this, &QxrdAcquisitionWindow::acquireStarted);
      connect(acqp.data(), &QxrdAcquisition::acquireComplete, this, &QxrdAcquisitionWindow::acquireComplete);

      acqp -> prop_ExposureTime() -> linkTo(this -> m_ExposureTime);
      acqp -> prop_SummedExposures() -> linkTo(this -> m_SummedExposures);
      acqp -> prop_SkippedExposures() -> linkTo(this -> m_SkippedExposures);
      acqp -> prop_SkippedExposuresAtStart() -> linkTo(this -> m_SkippedExposuresAtStart);
      acqp -> prop_DarkSummedExposures() -> linkTo(this -> m_DarkSummedExposures);
      acqp -> prop_FilePattern() -> linkTo(this -> m_FilePattern);
      acqp -> prop_FileIndex() -> linkTo(this -> m_FileIndex);
      acqp -> prop_PhasesInGroup() -> linkTo(this -> m_PhasesInGroup);
      acqp -> prop_PreTriggerFiles() -> linkTo(this -> m_PreTriggerFiles);
      acqp -> prop_PostTriggerFiles() -> linkTo(this -> m_PostTriggerFiles);
      acqp -> prop_DroppedFrames() -> linkTo(this -> m_DroppedFrames);

      acqp -> prop_UserComment1() -> linkTo(this -> m_UserComment1);
      acqp -> prop_UserComment2() -> linkTo(this -> m_UserComment2);
      acqp -> prop_UserComment3() -> linkTo(this -> m_UserComment3);
      acqp -> prop_UserComment4() -> linkTo(this -> m_UserComment4);

      acqp -> prop_LiveViewAtIdle() -> linkTo(this -> m_LiveViewAtIdle);
      acqp -> prop_AcquisitionCancelsLiveView() -> linkTo(this -> m_AcquisitionCancelsLiveView);
      acqp -> prop_RetryDropped() -> linkTo(this -> m_RetryDropped);
    }
  }

  connect(m_AcquireOptionsButton, &QPushButton::clicked, this, &QxrdAcquisitionWindow::doEditCorrection);
  connect(m_DetectorOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditDetectorPreferences);
  connect(m_AcquireOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditPreferences);
}

QxrdAcquisitionWindow::~QxrdAcquisitionWindow()
{
}

void QxrdAcquisitionWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdAcquisitionWindow::doEditCorrection()
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QxrdAcquisitionPtr   acq  = exp->acquisition();
    QxrdDataProcessorPtr proc = exp->dataProcessor();

    if (acq && proc) {
      QxrdCorrectionDialog* editCorrection =
          new QxrdCorrectionDialog(this, acq, proc);

      editCorrection->exec();
    }
  }
}


void QxrdAcquisitionWindow::acquireStarted()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    m_AcquireButton -> setEnabled(false);
    m_ActionAcquire -> setEnabled(false);

    m_AcquireOnceButton -> setEnabled(false);
    m_ActionAcquireOnce -> setEnabled(false);

    if (acq -> get_PreTriggerFiles() > 0) {
      m_TriggerButton -> setEnabled(true);
      m_ActionTrigger -> setEnabled(true);
    } else {
      m_TriggerButton -> setEnabled(false);
      m_ActionTrigger -> setEnabled(false);
    }

    m_CancelButton -> setEnabled(true);
    m_ActionCancel -> setEnabled(true);

    m_DarkAcquireButton -> setEnabled(false);
    m_ActionAcquireDark -> setEnabled(false);
  }
}

void QxrdAcquisitionWindow::acquireComplete()
{
  m_AcquireButton -> setEnabled(true);
  m_ActionAcquire -> setEnabled(true);

  m_AcquireOnceButton -> setEnabled(true);
  m_ActionAcquireOnce -> setEnabled(true);

  m_TriggerButton -> setEnabled(false);
  m_ActionTrigger -> setEnabled(false);

  m_CancelButton -> setEnabled(false);
  m_ActionCancel -> setEnabled(false);

  m_DarkAcquireButton -> setEnabled(true);
  m_ActionAcquireDark -> setEnabled(true);
}

void QxrdAcquisitionWindow::browseLogFile()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_LogFileName->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_LogFileName->setText(pwd.relativeFilePath(file));
  }
}

void QxrdAcquisitionWindow::browseScanFile()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_ScanFileName->text());

  QString file = QFileDialog::getSaveFileName(this, "Scan File", initial.absoluteFilePath());

  if (file != "") {
    m_ScanFileName->setText(pwd.relativeFilePath(file));
  }
}
