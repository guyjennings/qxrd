#include "qxrdacquisitiondialog.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include <QFileDialog>
#include <QDir>

QxrdAcquisitionDialog::QxrdAcquisitionDialog(QxrdExperimentWPtr doc,
                                     QxrdWindowWPtr win,
                                     QxrdAcquisitionWPtr acq,
                                     QxrdDataProcessorWPtr proc,
                                     QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(doc),
  m_Window(win),
  m_Acquisition(acq),
  m_DataProcessor(proc)
{
  setupUi(this);

  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    connect(m_ActionAcquire, &QAction::triggered, this, &QxrdAcquisitionDialog::doAcquire);
    connect(m_ActionCancel, &QAction::triggered, this, &QxrdAcquisitionDialog::doCancel);
    connect(m_ActionAcquireDark, &QAction::triggered, this, &QxrdAcquisitionDialog::doAcquireDark);
    connect(m_ActionTrigger, &QAction::triggered, acqp.data(), &QxrdAcquisition::trigger);

    connect(m_BrowseLogFileButton, &QAbstractButton::clicked, this, &QxrdAcquisitionDialog::browseLogFile);
    connect(m_BrowseScanFileButton, &QAbstractButton::clicked, this, &QxrdAcquisitionDialog::browseScanFile);

    connect(m_AcquireButton, &QAbstractButton::clicked, m_ActionAcquire, &QAction::triggered);
    connect(m_CancelButton, &QAbstractButton::clicked, m_ActionCancel, &QAction::triggered);
    connect(m_TriggerButton, &QAbstractButton::clicked, m_ActionTrigger, &QAction::triggered);
    connect(m_DarkAcquireButton, &QAbstractButton::clicked, m_ActionAcquireDark, &QAction::triggered);

    connect(m_ClearDroppedButton, &QAbstractButton::clicked, acqp.data(), &QxrdAcquisition::clearDropped);

    connect(acqp.data(), &QxrdAcquisition::acquireStarted, this, &QxrdAcquisitionDialog::acquireStarted);
    connect(acqp.data(), &QxrdAcquisition::acquireComplete, this, &QxrdAcquisitionDialog::acquireComplete);

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

  QxrdExperimentPtr expp(m_Experiment);

  if (expp) {
    expp  -> prop_ExperimentDirectory() -> linkTo(this -> m_ExperimentDirectory);
    expp  -> prop_LogFileName() -> linkTo(this -> m_LogFileName);
    expp  -> prop_DataDirectory() -> linkTo(this -> m_DataDirectory);
    expp  -> prop_ScanFileName() -> linkTo(this -> m_ScanFileName);
//    exp  -> prop_DetectorTypeName() -> linkTo(this -> m_DetectorTypeNameLabel);
//    exp  -> prop_DetectorNumber() -> linkTo(this -> m_DetectorNumber);

    connect(m_LogFileName, &QLineEdit::editingFinished, expp.data(), &QxrdExperiment::openNewLogFile);
  }

  QxrdDataProcessorPtr procp(m_DataProcessor);

  if (procp) {
    procp -> prop_Average()                -> linkTo(this -> m_AverageDisplay);
    procp -> prop_CorrectionQueueLength()  -> linkTo(this -> m_CorrectionBacklog);
    procp -> prop_IntegrationQueueLength() -> linkTo(this -> m_IntegrationBacklog);
    procp -> prop_SaverQueueLength()       -> linkTo(this -> m_SaverBacklog);
  }

  QxrdWindowPtr wp = m_Window;

  if (wp) {
    connect(m_DetectorOptionsButton, &QAbstractButton::clicked, wp.data(), &QxrdWindow::doEditDetectorPreferences);
    connect(m_AcquireOptionsButton, &QAbstractButton::clicked, wp.data(), &QxrdWindow::doEditPreferences);
  }
}

QxrdAcquisitionDialog::~QxrdAcquisitionDialog()
{
}

void QxrdAcquisitionDialog::setupDetectorsMenu(QMenu *menu)
{
  menu->addSeparator();
  menu->addAction(m_ActionAcquire);
  menu->addAction(m_ActionTrigger);
  menu->addAction(m_ActionAcquireDark);
  menu->addAction(m_ActionCancel);
}

void QxrdAcquisitionDialog::onAcquisitionInit()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> setupExposureMenu(this -> m_ExposureTime);
  }
}

void QxrdAcquisitionDialog::browseOutputDirectory()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", m_ExperimentDirectory->text(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    m_ExperimentDirectory->setText(dir);
  }
}

void QxrdAcquisitionDialog::browseLogFile()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_LogFileName->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_LogFileName->setText(pwd.relativeFilePath(file));
  }
}

void QxrdAcquisitionDialog::browseScanFile()
{
  QDir pwd(m_ExperimentDirectory->text());
  QFileInfo initial(pwd, m_ScanFileName->text());

  QString file = QFileDialog::getSaveFileName(this, "Scan File", initial.absoluteFilePath());

  if (file != "") {
    m_ScanFileName->setText(pwd.relativeFilePath(file));
  }
}

void QxrdAcquisitionDialog::changeEvent(QEvent *e)
{
  QDockWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdAcquisitionDialog::doAcquire()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquire();
  }
}

void QxrdAcquisitionDialog::doAcquireDark()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquireDark();
  }
}

void QxrdAcquisitionDialog::doCancel()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> cancel();
  }
}

void QxrdAcquisitionDialog::acquisitionReady()
{
  m_AcquireButton -> setEnabled(true);
  m_ActionAcquire -> setEnabled(true);

  m_TriggerButton -> setEnabled(false);
  m_ActionTrigger -> setEnabled(false);

  m_CancelButton -> setEnabled(false);
  m_ActionCancel -> setEnabled(false);

  m_DarkAcquireButton -> setEnabled(true);
  m_ActionAcquireDark -> setEnabled(true);
}

void QxrdAcquisitionDialog::acquireStarted()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    m_AcquireButton -> setEnabled(false);
    m_ActionAcquire -> setEnabled(false);

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

void QxrdAcquisitionDialog::acquireComplete()
{
  m_AcquireButton -> setEnabled(true);
  m_ActionAcquire -> setEnabled(true);

  m_TriggerButton -> setEnabled(false);
  m_ActionTrigger -> setEnabled(false);

  m_CancelButton -> setEnabled(false);
  m_ActionCancel -> setEnabled(false);

  m_DarkAcquireButton -> setEnabled(true);
  m_ActionAcquireDark -> setEnabled(true);
}
