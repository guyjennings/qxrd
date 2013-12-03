#include "qxrdsynchronizedacquisitiondialog.h"
#include "qxrdsynchronizedacquisition.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdacquisition.h"
#include "qxrddebug.h"

QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(
    QxrdSynchronizedAcquisitionDialogSettingsWPtr settings,
    QWidget *parent,
    QxrdAcquisitionWPtr acqw) :

  QDockWidget(parent),
  m_DialogSettings(settings),
  m_Acquisition(acqw),
  m_SynchronizedAcquisition()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSynchronizedAcquisitionDialog::QxrdSynchronizedAcquisitionDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdSynchronizedAcquisitionDialogSettingsPtr set(m_DialogSettings);

  if (set) {
    m_WaveformPlot->init(set->synchronizedAcquisitionPlotSettings());
  }

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
      QxrdNIDAQPluginInterfacePtr nidaq = sync->nidaqPlugin();

      if (nidaq) {
        QStringList devices = nidaq->deviceNames();

        foreach(QString device, devices) {
          QString desc = nidaq->deviceType(device);
          int     isSim = nidaq->deviceIsSimulated(device);

          QString item = device+" : "+desc;

          if (isSim) {
            item += " [simulated]";
          }

          m_SyncAcqOutDevice->addItem(item, device);
        }
      }

      sync -> prop_SyncAcquisitionMode()          -> linkTo(m_SyncAcqMode);
      sync -> prop_SyncAcquisitionWaveform()      -> linkTo(m_SyncAcqWfm);
      sync -> prop_SyncAcquisitionOutputDevice()  -> linkTo(m_SyncAcqOutDevice);
      sync -> prop_SyncAcquisitionOutputChannel() -> linkTo(m_SyncAcqOutChan);

      sync -> prop_SyncAcquisitionMinimum()       -> linkTo(m_SyncAcqMinimum);
      sync -> prop_SyncAcquisitionMaximum()       -> linkTo(m_SyncAcqMaximum);
      sync -> prop_SyncAcquisitionSymmetry()      -> linkTo(m_SyncAcqSymmetry);
      sync -> prop_SyncAcquisitionPhaseShift()      -> linkTo(m_SyncAcqPhaseShift);

      connect(sync -> prop_SyncAcquisitionOutputDevice(), SIGNAL(valueChanged(QString,int)), this, SLOT(deviceChanged()));
      connect(sync -> prop_SyncAcquisitionOutputChannel(), SIGNAL(valueChanged(QString,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionMode(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionWaveform(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionMinimum(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionMaximum(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionSymmetry(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
      connect(sync -> prop_SyncAcquisitionPhaseShift(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    }

    connect(acq->prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(waveformChanged()));
    connect(acq->prop_PhasesInGroup(), SIGNAL(valueChanged(int,int)), this, SLOT(waveformChanged()));

    deviceChanged();
    waveformChanged();
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

void QxrdSynchronizedAcquisitionDialog::deviceChanged()
{
  QxrdSynchronizedAcquisitionPtr sync(m_SynchronizedAcquisition);

  if (sync) {
    QxrdNIDAQPluginInterfacePtr nidaq = sync->nidaqPlugin();

    if (nidaq) {
      QStringList aoChannels = nidaq->deviceAOChannels(sync->get_SyncAcquisitionOutputDevice());

      bool blocked = m_SyncAcqOutChan->blockSignals(true);
      m_SyncAcqOutChan->clear();

      foreach(QString chan, aoChannels) {
        m_SyncAcqOutChan->addItem(chan, chan);
      }

      m_SyncAcqOutChan->blockSignals(blocked);
    }
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
