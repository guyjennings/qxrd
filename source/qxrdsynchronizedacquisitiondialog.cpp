#include "qxrdsynchronizedacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"
#include "qwt_plot_piecewise_curve.h"

QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(QWidget *parent, QxrdAcquisitionWPtr acqw) :
  QDockWidget(parent),
  m_Acquisition(acqw),
  m_SynchronizedAcquisition(NULL)
{
  setupUi(this);

  QxrdAcquisitionPtr acq(acqw);

  if (acq) {
    m_SynchronizedAcquisition = acq->synchronizedAcquisition();

    m_SyncAcqMode     -> addItem("No External Synchronization");
    m_SyncAcqMode     -> addItem("Stepped Output Synchronization");
    m_SyncAcqMode     -> addItem("Continuous Output Synchronization");

    m_SyncAcqWfm      -> addItem("Square Wave Output Waveform");
    m_SyncAcqWfm      -> addItem("Sine Wave Output Waveform");
    m_SyncAcqWfm      -> addItem("Triangle Wave Output Waveform");
    m_SyncAcqWfm      -> addItem("Sawtooth Wave Output Waveform");
    m_SyncAcqWfm      -> addItem("Bipolar Triangle Wave Output Waveform");

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

    m_SyncAcqPhaseShift -> setMinimum(-100.0);
    m_SyncAcqPhaseShift -> setMaximum(100.0);
    m_SyncAcqPhaseShift -> setSingleStep(1);

    m_SynchronizedAcquisition -> prop_SyncAcquisitionMode()          -> linkTo(m_SyncAcqMode);
    m_SynchronizedAcquisition -> prop_SyncAcquisitionWaveform()      -> linkTo(m_SyncAcqWfm);
    m_SynchronizedAcquisition -> prop_SyncAcquisitionOutputChannel() -> linkTo(m_SyncAcqOutChan);
    m_SynchronizedAcquisition -> prop_SyncAcquisitionFlagChannel()   -> linkTo(m_SyncAcqFlagChan);

    m_SynchronizedAcquisition -> prop_SyncAcquisitionMinimum()       -> linkTo(m_SyncAcqMinimum);
    m_SynchronizedAcquisition -> prop_SyncAcquisitionMaximum()       -> linkTo(m_SyncAcqMaximum);
    m_SynchronizedAcquisition -> prop_SyncAcquisitionSymmetry()      -> linkTo(m_SyncAcqSymmetry);
    m_SynchronizedAcquisition -> prop_SyncAcquisitionPhaseShift()      -> linkTo(m_SyncAcqPhaseShift);

    connect(m_SynchronizedAcquisition -> prop_SyncAcquisitionMode(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
    connect(m_SynchronizedAcquisition -> prop_SyncAcquisitionWaveform(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
    connect(m_SynchronizedAcquisition -> prop_SyncAcquisitionMinimum(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(m_SynchronizedAcquisition -> prop_SyncAcquisitionMaximum(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(m_SynchronizedAcquisition -> prop_SyncAcquisitionSymmetry(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(m_SynchronizedAcquisition -> prop_SyncAcquisitionPhaseShift(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));

    connect(acq->prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(acq->prop_PhasesInGroup(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
  }
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

void QxrdSynchronizedAcquisitionDialog::waveformChanged()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq && m_SynchronizedAcquisition) {
    QxrdAcquisition::QxrdAcquisitionParameterPack parms = acq->acquisitionParameterPack();
    m_SynchronizedAcquisition->prepareForAcquisition(&parms);

    m_WaveformPlot->clear();

    if (m_SynchronizedAcquisition -> get_SyncAcquisitionMode() && (parms.nphases()>=2)) {
      QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(m_WaveformPlot, "Output Waveform");

      pc->setData(m_SynchronizedAcquisition->outputTimes(), m_SynchronizedAcquisition->outputVoltage());

      pc->attach(m_WaveformPlot);
    }

    m_WaveformPlot->updateZoomer();
    m_WaveformPlot->replot();

    m_SynchronizedAcquisition->finishedAcquisition();
  }
}
