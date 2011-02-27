#include "qxrdsynchronizedacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"

QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(QWidget *parent, QxrdAcquisition *acq) :
    QDockWidget(parent),
    m_SynchronizedAcquisition(acq->synchronizedAcquisition())
{
  setupUi(this);

  m_SyncAcqMode     -> addItem("No External Synchronization");
  m_SyncAcqMode     -> addItem("Stepped Output Synchronization");
  m_SyncAcqMode     -> addItem("Continuous Output Synchronization");

  m_SyncAcqWfm      -> addItem("Square Wave Output Waveform");
  m_SyncAcqWfm      -> addItem("Sine Wave Output Waveform");
  m_SyncAcqWfm      -> addItem("Triangle Wave Output Waveform");
  m_SyncAcqWfm      -> addItem("Sawtooth Wave Output Waveform");

  m_SyncAcqOutChan  -> addItem("No Analog Output");
  m_SyncAcqOutChan  -> addItem("Analog Output 0 (AO0)");
  m_SyncAcqOutChan  -> addItem("Analog Output 1 (AO1)");

  m_SyncAcqFlagChan -> addItem("No Digital Output");

  m_SyncAcqMinimum  -> setMinimum(-10.0);
  m_SyncAcqMinimum  -> setMaximum(10.0);
  m_SyncAcqMinimum  -> setSingleStep(0.1);

  m_SyncAcqMaximum  -> setMinimum(-10.0);
  m_SyncAcqMaximum  -> setMaximum(10.0);
  m_SyncAcqMaximum  -> setSingleStep(0.1);

  m_SyncAcqSymmetry -> setMinimum(-1.0);
  m_SyncAcqSymmetry -> setMaximum(1.0);
  m_SyncAcqSymmetry -> setSingleStep(0.1);

  m_SynchronizedAcquisition -> prop_SyncAcquisitionMode()          -> linkTo(m_SyncAcqMode);
  m_SynchronizedAcquisition -> prop_SyncAcquisitionWaveform()      -> linkTo(m_SyncAcqWfm);
  m_SynchronizedAcquisition -> prop_SyncAcquisitionOutputChannel() -> linkTo(m_SyncAcqOutChan);
  m_SynchronizedAcquisition -> prop_SyncAcquisitionFlagChannel()   -> linkTo(m_SyncAcqFlagChan);

  m_SynchronizedAcquisition -> prop_SyncAcquisitionMinimum()       -> linkTo(m_SyncAcqMinimum);
  m_SynchronizedAcquisition -> prop_SyncAcquisitionMaximum()       -> linkTo(m_SyncAcqMaximum);
  m_SynchronizedAcquisition -> prop_SyncAcquisitionSymmetry()      -> linkTo(m_SyncAcqSymmetry);
}

QxrdSynchronizedAcquisitionDialog::~QxrdSynchronizedAcquisitionDialog()
{
}

void QxrdSynchronizedAcquisitionDialog::changeEvent(QEvent *e)
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
