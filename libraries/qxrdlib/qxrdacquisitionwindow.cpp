#include "qxrdacquisitionwindow.h"
#include "qxrdexperiment.h"
#include "qxrdacquisitionwindowsettings.h"
#include "qxrddetectorlistmodel.h"
#include "qxrdacqcommon.h"
#include "qxrdcorrectiondialog.h"
#include <QTableView>
#include <QDir>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include "qxrdprocessor.h"
#include "qxrdacquisitioneventlogwindow.h"
#include "qxrdinfowindow.h"

QxrdAcquisitionWindow::QxrdAcquisitionWindow(QString name) :
  inherited(name)
{
}

void QxrdAcquisitionWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  m_Settings    = qSharedPointerDynamicCast<QxrdAcquisitionWindowSettings>(m_Parent);
  m_Experiment  = QxrdExperiment::findExperiment(m_Parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp == NULL) {
    printMessage("NULL Experiment opening Acquisition Window");
  }

  if (exp) {
    m_Acquisition = exp -> acquisition();
    m_Processor   = exp -> processor();

    if (m_Acquisition == NULL) {
      printMessage("NULL Acquisition opening Acquisition Window");
    }

    if (m_Processor == NULL) {
      printMessage("NULL Processor opening Acquisition Window");
    }

    if (m_Settings == NULL) {
      printMessage("NULL Settings opening Acquisition Window");
    }

    m_DatasetBrowserView -> setExperiment(exp);

    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdAcquisitionWindowSettingsPtr settings(m_Settings);
    QxrdProcessorPtr                 procw(m_Processor);

    if (settings && procw) {
      m_FileBrowserWidget -> initialize(settings->fileBrowserSettings(), exp, procw);
    }

    m_DetectorsModel =
        QxrdDetectorListModelPtr(new QxrdDetectorListModel());

    QxrdAcqCommonPtr acqp(m_Acquisition);

    if (acqp) {
      for (int i=0; i<acqp->detectorCount(); i++) {
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

    CONNECT_CHECK(connect(m_LogFileName, &QLineEdit::editingFinished, exp.data(), &QxrdExperiment::openNewLogFile));

    if (acqp) {
      CONNECT_CHECK(connect(m_ActionAcquire,     &QAction::triggered, acqp.data(), &QxrdAcqCommon::acquire));
      CONNECT_CHECK(connect(m_ActionAcquireOnce, &QAction::triggered, acqp.data(), &QxrdAcqCommon::acquireOnce));
      CONNECT_CHECK(connect(m_ActionCancel,      &QAction::triggered, acqp.data(), &QxrdAcqCommon::cancel));
      CONNECT_CHECK(connect(m_ActionAcquireDark, &QAction::triggered, acqp.data(), &QxrdAcqCommon::acquireDark));
      CONNECT_CHECK(connect(m_ActionTrigger,     &QAction::triggered, acqp.data(), &QxrdAcqCommon::trigger));

      CONNECT_CHECK(connect(m_BrowseLogFileButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::browseLogFile));
      CONNECT_CHECK(connect(m_BrowseScanFileButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::browseScanFile));

      CONNECT_CHECK(connect(m_AcquireButton, &QAbstractButton::clicked, m_ActionAcquire, &QAction::triggered));
      CONNECT_CHECK(connect(m_AcquireOnceButton, &QAbstractButton::clicked, m_ActionAcquireOnce, &QAction::triggered));
      CONNECT_CHECK(connect(m_CancelButton, &QAbstractButton::clicked, m_ActionCancel, &QAction::triggered));
      CONNECT_CHECK(connect(m_TriggerButton, &QAbstractButton::clicked, m_ActionTrigger, &QAction::triggered));
      CONNECT_CHECK(connect(m_DarkAcquireButton, &QAbstractButton::clicked, m_ActionAcquireDark, &QAction::triggered));

      CONNECT_CHECK(connect(m_ClearDroppedButton, &QAbstractButton::clicked, acqp.data(), &QxrdAcqCommon::clearDropped));

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

  CONNECT_CHECK(connect(m_RestartDetectorsButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::restartDetectors));
  CONNECT_CHECK(connect(m_AcquisitionInfoButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::acquisitionInfoWindow));
  CONNECT_CHECK(connect(m_ExposureOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditExposurePreferences));
  CONNECT_CHECK(connect(m_ProcessorOptionsButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::doEditCorrection));
  CONNECT_CHECK(connect(m_DetectorOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditDetectorPreferences));
  CONNECT_CHECK(connect(m_AcquireOptionsButton, &QAbstractButton::clicked, this, &QxrdMainWindow::doEditPreferences));
  CONNECT_CHECK(connect(m_EventLogButton, &QAbstractButton::clicked, this, &QxrdAcquisitionWindow::eventLogWindow));
}

QxrdAcquisitionWindow::~QxrdAcquisitionWindow()
{
}

void QxrdAcquisitionWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
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
    QxrdAcqCommonPtr acq(m_Acquisition);
    QxrdProcessorPtr proc(m_Processor);

    if (acq && proc) {
      QxrdCorrectionDialog* editCorrection =
          new QxrdCorrectionDialog(this, acq, proc);

      editCorrection->exec();
    }
  }
}


void QxrdAcquisitionWindow::acquireStarted()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

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

  QString file = QFileDialog::getSaveFileName(this, "Log File",
                                              initial.absoluteFilePath()/*,
                                              QString(),
                                              Q_NULLPTR,
                                              QFileDialog::DontUseNativeDialog*/);

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

void QxrdAcquisitionWindow::eventLogWindow()
{
  if (m_AcquisitionEventLog == NULL) {
    m_AcquisitionEventLog =
        QxrdAcquisitionEventLogWindowPtr(
          new QxrdAcquisitionEventLogWindow(m_Acquisition));
  }

  if (m_AcquisitionEventLog) {
    m_AcquisitionEventLog->show();
    m_AcquisitionEventLog->activateWindow();
    m_AcquisitionEventLog->raise();
  }
}

void QxrdAcquisitionWindow::restartDetectors()
{
  int reply = QMessageBox::question(this, "Restart Detector Hardware", "Do you want to restart\nthe detector hardware?",
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    QxrdAcqCommonPtr acq(m_Acquisition);

    if (acq) {
      INVOKE_CHECK(
            QMetaObject::invokeMethod(acq.data(), "restartDetectors"));
    }
  }
}

void QxrdAcquisitionWindow::acquisitionInfoWindow()
{
  if (m_InfoWindow == NULL) {
    m_InfoWindow =
        QxrdInfoWindowPtr(
          new QxrdInfoWindow("extraIOInfo", m_Acquisition));

    m_InfoWindow -> initialize(m_Acquisition);
  }

  if (m_InfoWindow) {
    m_InfoWindow -> show();
    m_InfoWindow -> raise();
  }
}
