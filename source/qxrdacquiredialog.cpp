#include "qxrdacquiredialog.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"

QxrdAcquireDialog::QxrdAcquireDialog(QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc, QWidget *parent) :
    QDockWidget(parent),
    m_Window(win),
    m_Acquisition(acq),
    m_DataProcessor(proc)
{
  setupUi(this);

  connect(m_ActionAcquire, SIGNAL(triggered()), m_Window, SLOT(doAcquire()));
  connect(m_ActionCancel, SIGNAL(triggered()), m_Window, SLOT(doCancel()));
  connect(m_ActionAcquireDark, SIGNAL(triggered()), m_Window, SLOT(doAcquireDark()));
//  connect(m_ActionCancelDark, SIGNAL(triggered()), m_Window, SLOT(doCancelDark()));
  connect(m_ActionTrigger, SIGNAL(triggered()), m_Acquisition, SLOT(trigger()));

  connect(m_SelectLogFileButton, SIGNAL(clicked()), m_Window, SLOT(selectLogFile()));
  connect(m_SelectDirectoryButton, SIGNAL(clicked()), m_Window, SLOT(selectOutputDirectory()));

  connect(m_AcquireButton, SIGNAL(clicked()), m_ActionAcquire, SIGNAL(triggered()));
  connect(m_CancelButton, SIGNAL(clicked()), m_ActionCancel, SIGNAL(triggered()));
  connect(m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));

  connect(m_ClearDroppedButton, SIGNAL(clicked()), m_Acquisition, SLOT(clearDropped()));

  connect(m_Acquisition, SIGNAL(acquireStarted()), this, SLOT(acquireStarted()));
  connect(m_Acquisition, SIGNAL(acquireComplete()), this, SLOT(acquireComplete()));

  m_Acquisition -> prop_DetectorTypeName() -> linkTo(this -> m_DetectorTypeNameLabel);
  m_Acquisition -> prop_ExposureTime() -> linkTo(this -> m_ExposureTime);
  m_Acquisition -> prop_SummedExposures() -> linkTo(this -> m_SummedExposures);
  m_Acquisition -> prop_SkippedExposures() -> linkTo(this -> m_SkippedExposures);
  m_Acquisition -> prop_SkippedExposuresAtStart() -> linkTo(this -> m_SkippedExposuresAtStart);
  m_Acquisition -> prop_DarkSummedExposures() -> linkTo(this -> m_DarkSummedExposures);
  m_Acquisition -> prop_FilePattern() -> linkTo(this -> m_FilePattern);
  m_Acquisition -> prop_FileIndex() -> linkTo(this -> m_FileIndex);
  m_Acquisition -> prop_PhasesInGroup() -> linkTo(this -> m_PhasesInGroup);
  m_Acquisition -> prop_PreTriggerFiles() -> linkTo(this -> m_PreTriggerFiles);
  m_Acquisition -> prop_PostTriggerFiles() -> linkTo(this -> m_PostTriggerFiles);
  m_Acquisition -> prop_CameraGain() -> linkTo(this -> m_CameraGain);
  m_Acquisition -> prop_BinningMode() -> linkTo(this -> m_BinningMode);
  m_Acquisition -> prop_DroppedFrames() -> linkTo(this -> m_DroppedFrames);

  m_Acquisition -> prop_UserComment1() -> linkTo(this -> m_UserComment1);
  m_Acquisition -> prop_UserComment2() -> linkTo(this -> m_UserComment2);
  m_Acquisition -> prop_UserComment3() -> linkTo(this -> m_UserComment3);
  m_Acquisition -> prop_UserComment4() -> linkTo(this -> m_UserComment4);

  m_DataProcessor -> prop_OutputDirectory() -> linkTo(this -> m_OutputDirectory);
  m_DataProcessor -> prop_LogFilePath() -> linkTo(this -> m_LogFilePath);
  m_DataProcessor -> prop_Average() -> linkTo(this -> m_AverageDisplay);
}

QxrdAcquireDialog::~QxrdAcquireDialog()
{
}

void QxrdAcquireDialog::onAcquisitionInit()
{
  m_Acquisition -> setupExposureMenu(this -> m_ExposureTime);
  m_Acquisition -> setupCameraGainMenu(this -> m_CameraGain);
  m_Acquisition -> setupCameraBinningModeMenu(this -> m_BinningMode);
}

void QxrdAcquireDialog::changeEvent(QEvent *e)
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

void QxrdAcquireDialog::acquireStarted()
{
  acquisitionStarted();
}

void QxrdAcquireDialog::acquireComplete()
{
  acquisitionFinished();
}

void QxrdAcquireDialog::acquisitionReady()
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

void QxrdAcquireDialog::acquisitionStarted()
{
  m_AcquireButton -> setEnabled(false);
  m_ActionAcquire -> setEnabled(false);

  if (m_Acquisition -> get_PreTriggerFiles() > 0) {
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

void QxrdAcquireDialog::acquisitionFinished()
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
