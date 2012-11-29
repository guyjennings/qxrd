#include "qxrdsynchronizedacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdacquisition.h"
#include "qxrddebug.h"

QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(QWidget *parent, QxrdAcquisitionWPtr acqw) :
  QDockWidget(parent),
  m_Acquisition(acqw),
  m_SynchronizedAcquisition()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdAcquisitionPtr acq(acqw);

  if (acq) {
    m_SynchronizedAcquisition = acq->synchronizedAcquisition();

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

    QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

    if (sync) {
      sync -> prop_SyncAcquisitionMode()          -> linkTo(m_SyncAcqMode);
      sync -> prop_SyncAcquisitionWaveform()      -> linkTo(m_SyncAcqWfm);
      sync -> prop_SyncAcquisitionOutputChannel() -> linkTo(m_SyncAcqOutChan);
      sync -> prop_SyncAcquisitionFlagChannel()   -> linkTo(m_SyncAcqFlagChan);

      sync -> prop_SyncAcquisitionMinimum()       -> linkTo(m_SyncAcqMinimum);
      sync -> prop_SyncAcquisitionMaximum()       -> linkTo(m_SyncAcqMaximum);
      sync -> prop_SyncAcquisitionSymmetry()      -> linkTo(m_SyncAcqSymmetry);
      sync -> prop_SyncAcquisitionPhaseShift()      -> linkTo(m_SyncAcqPhaseShift);

      connect(sync -> prop_SyncAcquisitionMode(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionWaveform(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionMinimum(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionMaximum(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionSymmetry(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionPhaseShift(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    }

    connect(acq->prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(acq->prop_PhasesInGroup(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
  }
}

QxrdSynchronizedAcquisitionDialog::~QxrdSynchronizedAcquisitionDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSynchronizedAcquisitionDialog::~QxrdSynchronizedAcquisitionDialog(%p)\n", this);
  }
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

void QxrdSynchronizedAcquisitionDialog::waveformChanged()
{
  QxrdAcquisitionPtr acq(m_Acquisition);
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (acq && sync) {
    QxrdAcquisitionParameterPack parms = acq->acquisitionParameterPack();
    sync->prepareForAcquisition(&parms);

    m_WaveformPlot->clear();

    if (sync -> get_SyncAcquisitionMode() && (parms.nphases()>=2)) {
      QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(m_WaveformPlot, "Output Waveform");

      pc->setData(sync->outputTimes(), sync->outputVoltage());

      pc->attach(m_WaveformPlot);
    }

    m_WaveformPlot->updateZoomer();
    m_WaveformPlot->replot();

    sync->finishedAcquisition();
  }
}
