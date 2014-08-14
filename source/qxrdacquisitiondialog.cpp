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
    QxrdAcquisition *acq = acqp.data();

    connect(m_ActionAcquire, SIGNAL(triggered()), this, SLOT(doAcquire()));
    connect(m_ActionCancel, SIGNAL(triggered()), this, SLOT(doCancel()));
    connect(m_ActionAcquireDark, SIGNAL(triggered()), this, SLOT(doAcquireDark()));
    //  connect(m_ActionCancelDark, SIGNAL(triggered()), this, SLOT(doCancelDark()));
    connect(m_ActionTrigger, SIGNAL(triggered()), acq, SLOT(trigger()));

//    connect(m_BrowseDirectoryButton, SIGNAL(clicked()), this, SLOT(browseOutputDirectory()));
    connect(m_BrowseLogFileButton, SIGNAL(clicked()), this, SLOT(browseLogFile()));
    connect(m_BrowseScanFileButton, SIGNAL(clicked()), this, SLOT(browseScanFile()));

    connect(m_AcquireButton, SIGNAL(clicked()), m_ActionAcquire, SIGNAL(triggered()));
    connect(m_CancelButton, SIGNAL(clicked()), m_ActionCancel, SIGNAL(triggered()));
    connect(m_TriggerButton, SIGNAL(clicked()), m_ActionTrigger, SIGNAL(triggered()));
    connect(m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));

    connect(m_ClearDroppedButton, SIGNAL(clicked()), acq, SLOT(clearDropped()));

    connect(acq, SIGNAL(acquireStarted()), this, SLOT(acquireStarted()));
    connect(acq, SIGNAL(acquireComplete()), this, SLOT(acquireComplete()));

    acq -> prop_ExposureTime() -> linkTo(this -> m_ExposureTime);
    acq -> prop_SummedExposures() -> linkTo(this -> m_SummedExposures);
    acq -> prop_SkippedExposures() -> linkTo(this -> m_SkippedExposures);
    acq -> prop_SkippedExposuresAtStart() -> linkTo(this -> m_SkippedExposuresAtStart);
    acq -> prop_DarkSummedExposures() -> linkTo(this -> m_DarkSummedExposures);
    acq -> prop_FilePattern() -> linkTo(this -> m_FilePattern);
    acq -> prop_FileIndex() -> linkTo(this -> m_FileIndex);
    acq -> prop_PhasesInGroup() -> linkTo(this -> m_PhasesInGroup);
    acq -> prop_PreTriggerFiles() -> linkTo(this -> m_PreTriggerFiles);
    acq -> prop_PostTriggerFiles() -> linkTo(this -> m_PostTriggerFiles);
    acq -> prop_CameraGain() -> linkTo(this -> m_CameraGain);
    acq -> prop_BinningMode() -> linkTo(this -> m_BinningMode);
    acq -> prop_DroppedFrames() -> linkTo(this -> m_DroppedFrames);

    acq -> prop_UserComment1() -> linkTo(this -> m_UserComment1);
    acq -> prop_UserComment2() -> linkTo(this -> m_UserComment2);
    acq -> prop_UserComment3() -> linkTo(this -> m_UserComment3);
    acq -> prop_UserComment4() -> linkTo(this -> m_UserComment4);

    acq -> prop_LiveViewAtIdle() -> linkTo(this -> m_LiveViewAtIdle);
    acq -> prop_AcquisitionCancelsLiveView() -> linkTo(this -> m_AcquisitionCancelsLiveView);
    acq -> prop_RetryDropped() -> linkTo(this -> m_RetryDropped);
  } else {
    printf("acq == NULL in QxrdAcquisitionDialog::QxrdAcquisitionDialog");
  }

  QxrdExperimentPtr expp(m_Experiment);

  if (expp) {
    QxrdExperiment *exp = expp.data();

    exp  -> prop_ExperimentDirectory() -> linkTo(this -> m_ExperimentDirectory);
    exp  -> prop_LogFileName() -> linkTo(this -> m_LogFileName);
    exp  -> prop_DataDirectory() -> linkTo(this -> m_DataDirectory);
    exp  -> prop_ScanFileName() -> linkTo(this -> m_ScanFileName);
    exp  -> prop_DetectorTypeName() -> linkTo(this -> m_DetectorTypeNameLabel);
    exp  -> prop_DetectorNumber() -> linkTo(this -> m_DetectorNumber);
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
    connect(m_DetectorOptionsButton, SIGNAL(clicked()), wp.data(), SLOT(doEditDetectorPreferences()));
    connect(m_AcquireOptionsButton, SIGNAL(clicked()), wp.data(), SLOT(doEditPreferences()));
  }
}

QxrdAcquisitionDialog::~QxrdAcquisitionDialog()
{
}

void QxrdAcquisitionDialog::setupAcquireMenu(QMenu *menu)
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
    acqp -> setupCameraGainMenu(this -> m_CameraGain);
    acqp -> setupCameraBinningModeMenu(this -> m_BinningMode);
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
