#include "qxrdacquisitiontriggerdialog.h"
#include "qxrdacquisition.h"
#include "ui_qxrdacquisitiontriggerdialog.h"
#include "qwt_plot_piecewise_curve.h"

QxrdAcquisitionTriggerDialog::QxrdAcquisitionTriggerDialog(QWidget *parent, QxrdAcquisitionWPtr acqw) :
  QDockWidget(parent),
  m_Acquisition(acqw),
  m_AcquisitionTrigger()
{
  setupUi(this);

  QxrdAcquisitionPtr acq(acqw);

  if (acq) {
    m_AcquisitionTrigger = acq->acquisitionTrigger();

    m_SyncAcqMode     -> addItem("None");
    m_SyncAcqMode     -> addItem("Stepped Output");
    m_SyncAcqMode     -> addItem("Continuous Output");

    m_SyncAcqWfm      -> addItem("Square");
    m_SyncAcqWfm      -> addItem("Sine");
    m_SyncAcqWfm      -> addItem("Triangle");
    m_SyncAcqWfm      -> addItem("Sawtooth");
    m_SyncAcqWfm      -> addItem("Bipolar Triangle");

    m_SyncAcqOutChan  -> addItem("None");
    m_SyncAcqOutChan  -> addItem("AO0");
    m_SyncAcqOutChan  -> addItem("AO1");

    m_SyncAcqFlagChan -> addItem("None");

    m_SyncAcqMinimum  -> setMinimum(-10.0);
    m_SyncAcqMinimum  -> setMaximum(10.0);
    m_SyncAcqMinimum  -> setSingleStep(0.1);

    m_SyncAcqMaximum  -> setMinimum(-10.0);
    m_SyncAcqMaximum  -> setMaximum(10.0);
    m_SyncAcqMaximum  -> setSingleStep(0.1);

    m_SyncAcqSymmetry -> setMinimum(-1.0);
    m_SyncAcqSymmetry -> setMaximum(1.0);
    m_SyncAcqSymmetry -> setSingleStep(0.1);

    m_SyncAcqPhaseShift -> setMinimum(-100.0);
    m_SyncAcqPhaseShift -> setMaximum(100.0);
    m_SyncAcqPhaseShift -> setSingleStep(1);

    connect(acq->prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(acq->prop_PhasesInGroup(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
  }
}

QxrdAcquisitionTriggerDialog::~QxrdAcquisitionTriggerDialog()
{
}

void QxrdAcquisitionTriggerDialog::changeEvent(QEvent *e)
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
