#include "qxrdacquiredialog.h"
#include "qxrdwindow.h"
#include "qxrdacquisition.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"

QxrdAcquireDialog::QxrdAcquireDialog(QxrdExperimentWPtr doc,
                                     QxrdWindow *win,
                                     QxrdAcquisitionWPtr acq,
                                     QxrdDataProcessorWPtr proc,
                                     QWidget *parent) :
  QxrdAcquireDialogBase(doc, win, acq, proc, parent)
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

    //  connect(m_SelectLogFileButton, SIGNAL(clicked()), m_Window, SLOT(selectLogFile()));
    //  connect(m_SelectDirectoryButton, SIGNAL(clicked()), m_Window, SLOT(selectOutputDirectory()));

    connect(m_AcquireButton, SIGNAL(clicked()), m_ActionAcquire, SIGNAL(triggered()));
    connect(m_CancelButton, SIGNAL(clicked()), m_ActionCancel, SIGNAL(triggered()));
    connect(m_TriggerButton, SIGNAL(clicked()), m_ActionTrigger, SIGNAL(triggered()));
    connect(m_DarkAcquireButton, SIGNAL(clicked()), m_ActionAcquireDark, SIGNAL(triggered()));

    connect(m_ClearDroppedButton, SIGNAL(clicked()), acq, SLOT(clearDropped()));

    connect(acq, SIGNAL(acquireStarted()), this, SLOT(acquireStarted()));
    connect(acq, SIGNAL(acquireComplete()), this, SLOT(acquireComplete()));

    acq -> prop_DetectorTypeName() -> linkTo(this -> m_DetectorTypeNameLabel);
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
  } else {
    printf("acq == NULL in QxrdAcquireDilaog::QxrdAcquireDialog");
  }

  QxrdExperimentPtr expp(m_Experiment);

  if (expp) {
    QxrdExperiment *exp = expp.data();

    exp  -> prop_ExperimentDirectory() -> linkTo(this -> m_ExperimentDirectory);
    exp  -> prop_LogFileName() -> linkTo(this -> m_LogFileName);
  }

  QxrdDataProcessorPtr procp(m_DataProcessor);

  if (procp) {
    procp.data() -> prop_Average() -> linkTo(this -> m_AverageDisplay);
  }

  QxrdWindow *wp = m_Window;

  if (wp) {
    connect(m_AcquireOptionsButton, SIGNAL(clicked()), wp, SLOT(doEditPreferences()));
  }
}

QxrdAcquireDialog::~QxrdAcquireDialog()
{
}

void QxrdAcquireDialog::setupAcquireMenu(QMenu *menu)
{
  menu->addSeparator();
  menu->addAction(m_ActionAcquire);
  menu->addAction(m_ActionTrigger);
  menu->addAction(m_ActionAcquireDark);
  menu->addAction(m_ActionCancel);
}

void QxrdAcquireDialog::onAcquisitionInit()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> setupExposureMenu(this -> m_ExposureTime);
    acqp -> setupCameraGainMenu(this -> m_CameraGain);
    acqp -> setupCameraBinningModeMenu(this -> m_BinningMode);
  }
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

void QxrdAcquireDialog::doAcquire()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquire();
  }
}

void QxrdAcquireDialog::doAcquireDark()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> acquireDark();
  }
}

void QxrdAcquireDialog::doCancel()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp -> cancel();
  }
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

void QxrdAcquireDialog::acquireStarted()
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

void QxrdAcquireDialog::acquireComplete()
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
