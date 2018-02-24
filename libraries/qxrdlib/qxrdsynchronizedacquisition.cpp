#include "qxrdsynchronizedacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrdacqcommon.h"
#include "qwt_math.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrdnidaq.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QString name) :
  QcepObject(name),
  m_SyncAcquisitionMode(this,"syncAcquisitionMode", 0, "Synchronized Acquisition Mode (0 = None, 1 = Stepped, 2 = Continuous)"),
  m_SyncAcquisitionWaveform(this,"syncAcquisitionWaveform", 0,
                            "Synchronized Acquisition Waveform (0 = Square, 1 = Sine, 2 = Triangle, 3 = Sawtooth, 4 = Bipolar Triangle)"),
  m_SyncAcquisitionOutputDevice(this,"syncAcquisitionOutputDevice", "", "Synchronized Acquisition Output Device"),
  m_SyncAcquisitionOutputChannel(this,"syncAcquisitionOutputChannel", "", "Synchronized Acquisition Output Channel"),
  m_SyncAcquisitionMinimum(this,"syncAcquisitionMinimum", 0.0, "Synchronized Acquisition Minimum (in Volts)"),
  m_SyncAcquisitionMaximum(this,"syncAcquisitionMaximum", 5.0, "Synchronized Acquisition Maximum (in Volts)"),
  m_SyncAcquisitionSymmetry(this,"syncAcquisitionSymmetry", 0.0, "Synchronized Acquisition Symmetry (0 = symmetric)"),
  m_SyncAcquisitionPhaseShift(this,"syncAcquisitionPhaseShift", 0.0, "Synchronized Acquisition Phase Shift (deg)"),
  m_SyncAcquisitionManualValue(this,"syncAcquisitionManualValue", 0.0, "Manual Output Voltage (in Volts)"),
  m_SyncMode(0)
{
#ifndef QT_NO_DEBUG
  printf("Constructing synchronized acquisition\n");
#endif
}

void QxrdSynchronizedAcquisition::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);
}

QxrdSynchronizedAcquisition::~QxrdSynchronizedAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deleting synchronized acquisition\n");
#endif
}

void QxrdSynchronizedAcquisition::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

//  int n = settings->beginReadArray("channels");

//  //TODO: rewrite...
//  while (m_Channels.count() > n) {
//    removeChannel();
//  }

//  while (m_Channels.count() < n) {
//    appendChannel();
//  }

//  for (int i=0; i<n; i++) {
//    settings->setArrayIndex(i);

//    QxrdAcquisitionExtraInputsChannelPtr chan = channel(i);

//    if (chan) {
//      chan->readSettings(settings);
//    }
//  }

//  settings->endArray();

//  initiate();
}

void QxrdSynchronizedAcquisition::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

//  settings->beginWriteArray("channels");

//  int n = m_Channels.count();

//  for (int i=0; i<n; i++) {
//    settings->setArrayIndex(i);
//    QxrdAcquisitionExtraInputsChannelPtr chan = channel(i);

//    if (chan) {
//      chan->writeSettings(settings);
//    }
//  }

//  settings->endArray();
}

void QxrdSynchronizedAcquisition::setNIDAQPlugin(QxrdNIDAQWPtr nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdAcqCommonWPtr QxrdSynchronizedAcquisition::acquisition()
{
  return m_Acquisition;
}

QxrdNIDAQWPtr QxrdSynchronizedAcquisition::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

QxrdAcquisitionParameterPackWPtr QxrdSynchronizedAcquisition::parms()
{
  return m_AcquisitionParms;
}

void QxrdSynchronizedAcquisition::finishedAcquisition()
{
  m_AcquisitionParms = QxrdAcquisitionParameterPackWPtr();
  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::prepareForDarkAcquisition(QxrdDarkAcquisitionParameterPackWPtr /*parms*/)
{
  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::prepareForAcquisition(QxrdAcquisitionParameterPackWPtr parms)
{
  m_AcquisitionParms = parms;

  QxrdAcquisitionParameterPackPtr parmsp(parms);

  if (parmsp) {
    m_SyncMode = 0;
    m_AcquisitionParms = parms;

    double exposureTime = parmsp->exposure();
    int    nphases      = parmsp->nphases();
    double cycleTime    = exposureTime/**nsummed*/*nphases;
    double sampleRate   = 1000;
    double nSamples     = cycleTime*sampleRate;
    double minVal       = get_SyncAcquisitionMinimum();
    double maxVal       = get_SyncAcquisitionMaximum();
    QString chan        = get_SyncAcquisitionOutputChannel();
    int wfm             = get_SyncAcquisitionWaveform();
    m_SyncMode          = get_SyncAcquisitionMode();
    double symm         = get_SyncAcquisitionSymmetry();
    double phase        = get_SyncAcquisitionPhaseShift();

    if (symm > 1.0) {
      symm = 1.0;
    } else if (symm < -1.0) {
      symm = -1.0;
    }

    if (nphases <= 0) {
      m_SyncMode = 0;
    } else if (m_SyncMode) {

      while (nSamples > 10000) {
        sampleRate /= 10;
        nSamples = cycleTime*sampleRate;
      }

      int iSamples = (int) nSamples;
      double divide = iSamples * (0.5 + symm/2.0);
      double divideBy2 = divide/2;
      int shift = (int)((double) phase*iSamples/360.0 /*+ nphases*/) % iSamples;

      QVector<double> outputTimes(iSamples+1);
      QVector<double> outputVoltage(iSamples+1);

      for (int i=0; i<=iSamples; i++) {
        outputTimes[i] = ((double)i)/((double) sampleRate);
      }

      switch (wfm) {
      case SyncAcquisitionWaveformSquare:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          if (i<divide) {
            outputVoltage[ii] = minVal;
          } else {
            outputVoltage[ii] = maxVal;
          }
        }
        break;

      case SyncAcquisitionWaveformSine:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          double x;
          if (i<divide) {
            x = M_PI*i/divide;
          } else {
            x = M_PI+M_PI*(i-divide)/(iSamples-divide);
          }
          outputVoltage[ii] = minVal + (maxVal-minVal)*(1.0 - cos(x))/2.0;
        }
        break;

      case SyncAcquisitionWaveformTriangle:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          if (i<divide) {
            outputVoltage[ii] = minVal + i*(maxVal-minVal)/divide;
          } else {
            outputVoltage[ii] = maxVal - (i-divide)*(maxVal-minVal)/(iSamples-divide);
          }
        }
        break;

      case SyncAcquisitionWaveformBipolarTriangle:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          if (i < divideBy2) {
            outputVoltage[ii] = minVal + i*(maxVal-minVal)/divideBy2;
          } else if (i < (iSamples-divideBy2)) {
            outputVoltage[ii] = maxVal - (i-divideBy2)*(maxVal-minVal)/((iSamples-divide)/2);
          } else {
            outputVoltage[ii] = minVal - (iSamples-i)*(maxVal-minVal)/divideBy2;
          }
        }
        break;

      case SyncAcquisitionWaveformSawtooth:
      default:
        for (int ii=0; ii<iSamples; ii++) {
          int i = (ii+iSamples-shift) % iSamples;
          outputVoltage[ii] = minVal + i*(maxVal-minVal)/iSamples;
        }
        break;
      }

      outputVoltage[iSamples] = outputVoltage[0]; // Return output voltage to starting value at the end of the waveform

      QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

      if (nidaq) {
        nidaq->setAnalogWaveform(chan, sampleRate, outputVoltage.data(), iSamples+1);
      }

      m_OutputTimes = outputTimes;
      m_OutputVoltage = outputVoltage;
    }
  }
}

//static QTime tick;

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int frameNumber)
{
  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    nidaq->pulseOutput();
  }

  QxrdAcqCommonPtr acq(m_Acquisition);
  QxrdAcquisitionParameterPackPtr parms(m_AcquisitionParms);

  if (m_SyncMode && acq && parms) {
    if (acq->acquisitionStatus(0.0) == 0) {
//      printf("QxrdSynchronizedAcquisition::acquiredFrameAvailable(%d)\n", frameNumber);

      int skipBefore = parms->skipBefore();
      int skipBetween = parms->skipBetween();
      int nPhases = parms->nphases();
      int nSummed = parms->nsummed();
      int nGroups = parms->postTrigger();
      int perGroup = nPhases*nSummed+skipBetween;
      int inGroup = (frameNumber-skipBefore) % perGroup;
      int phase = inGroup % nPhases;

      if (nPhases > 0) {
        if ((frameNumber >= skipBefore) && (frameNumber < (nGroups*perGroup-skipBetween+skipBefore))) {
          if (inGroup < nPhases*nSummed) {
            if (phase == 0) {
              if (nidaq) {
//                printf("Triggered on frame %d\n", frameNumber);
                nidaq->triggerAnalogWaveform();
              }
            }
          }
        }
      }
//      printf("elapsed[%d] %d msec\n", currentPhase, tick.restart());
    }
  }
}

void QxrdSynchronizedAcquisition::setManualOutput()
{
  QxrdAcqCommonPtr acq(m_Acquisition);
  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (acq && nidaq) {
    QString fullChannel = get_SyncAcquisitionOutputChannel();

    acq->printMessage(tr("Manually Setting %1 to %2 V")
                                .arg(fullChannel)
                                .arg(get_SyncAcquisitionManualValue()));

    nidaq->setAnalogOutput(fullChannel, get_SyncAcquisitionManualValue());
  }
}

void QxrdSynchronizedAcquisition::triggerOnce()
{
  QxrdAcqCommonPtr acq(m_Acquisition);
  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);
  QxrdAcquisitionParameterPackPtr parms(m_AcquisitionParms);

  if (acq && nidaq && parms) {
    prepareForAcquisition(parms);
    nidaq->triggerAnalogWaveform();
  }
}

QVector<double>  QxrdSynchronizedAcquisition::outputTimes()
{
  return m_OutputTimes;
}

QVector<double>  QxrdSynchronizedAcquisition::outputVoltage()
{
  return m_OutputVoltage;
}
