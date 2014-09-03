#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"
#include "qwt_math.h"
#include "qxrdsettingssaver.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdSettingsSaverPtr saver, QxrdAcquisition *acq) :
  QcepObject("synchronization", NULL),
  m_SyncAcquisitionMode(saver, this,"syncAcquisitionMode", 0, "Synchronized Acquisition Mode (0 = None, 1 = Stepped, 2 = Continuous)"),
  m_SyncAcquisitionWaveform(saver, this,"syncAcquisitionWaveform", 0,
                            "Synchronized Acquisition Waveform (0 = Square, 1 = Sine, 2 = Triangle, 3 = Sawtooth, 4 = Bipolar Triangle)"),
  m_SyncAcquisitionOutputDevice(saver, this,"syncAcquisitionOutputDevice", "", "Synchronized Acquisition Output Device"),
  m_SyncAcquisitionOutputChannel(saver, this,"syncAcquisitionOutputChannel", "", "Synchronized Acquisition Output Channel"),
//  m_SyncAcquisitionFlagChannel(saver, this,"syncAcquisitionFlagChannel", 0, "Synchronized Acquisition Flags"),
  m_SyncAcquisitionMinimum(saver, this,"syncAcquisitionMinimum", 0.0, "Synchronized Acquisition Minimum (in Volts)"),
  m_SyncAcquisitionMaximum(saver, this,"syncAcquisitionMaximum", 5.0, "Synchronized Acquisition Maximum (in Volts)"),
  m_SyncAcquisitionSymmetry(saver, this,"syncAcquisitionSymmetry", 0.0, "Synchronized Acquisition Symmetry (0 = symmetric)"),
  m_SyncAcquisitionPhaseShift(saver, this,"syncAcquisitionPhaseShift", 0.0, "Synchronized Acquisition Phase Shift (deg)"),
  m_SyncAcquisitionManualValue(saver, this,"syncAcquisitionManualValue", 0.0, "Manual Output Voltage (in Volts)"),
  m_Acquisition(acq),
  m_AcquisitionParms(NULL),
  m_NIDAQPlugin(NULL),
  m_SyncMode(0)
{
}

void QxrdSynchronizedAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdNIDAQPluginInterfacePtr QxrdSynchronizedAcquisition::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

void QxrdSynchronizedAcquisition::finishedAcquisition()
{
  m_AcquisitionParms = NULL;
  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::prepareForDarkAcquisition(QxrdDarkAcquisitionParameterPack * /*parms*/)
{
  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::prepareForAcquisition(QxrdAcquisitionParameterPack *parms)
{
  m_SyncMode = 0;
  m_AcquisitionParms = parms;

  double exposureTime = parms->exposure();
  int    nphases      = parms->nphases();
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

  if (nphases <= 1) {
    m_SyncMode = 0;
  } else if (m_SyncMode) {

    while (nSamples > 10000) {
      sampleRate /= 10;
      nSamples = cycleTime*sampleRate;
    }

    int iSamples = (int) nSamples;
    double divide = iSamples * (0.5 + symm/2.0);
    double divideBy2 = divide/2;
    int shift = (int)((double) phase*iSamples/360.0 + nphases) % iSamples;

    m_OutputTimes.resize(iSamples+1);
    m_OutputVoltage.resize(iSamples+1);

    for (int i=0; i<=iSamples; i++) {
      m_OutputTimes[i] = ((double)i)/((double) sampleRate);
    }

    switch (wfm) {
    case SyncAcquisitionWaveformSquare:
      for (int ii=0; ii<iSamples; ii++) {
        int i = (ii+iSamples-shift) % iSamples;
        if (i<divide) {
          m_OutputVoltage[ii] = minVal;
        } else {
          m_OutputVoltage[ii] = maxVal;
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
        m_OutputVoltage[ii] = minVal + (maxVal-minVal)*(1.0 - cos(x))/2.0;
      }
      break;

    case SyncAcquisitionWaveformTriangle:
      for (int ii=0; ii<iSamples; ii++) {
        int i = (ii+iSamples-shift) % iSamples;
        if (i<divide) {
          m_OutputVoltage[ii] = minVal + i*(maxVal-minVal)/divide;
        } else {
          m_OutputVoltage[ii] = maxVal - (i-divide)*(maxVal-minVal)/(iSamples-divide);
        }
      }
      break;

    case SyncAcquisitionWaveformBipolarTriangle:
      for (int ii=0; ii<iSamples; ii++) {
        int i = (ii+iSamples-shift) % iSamples;
        if (i < divideBy2) {
          m_OutputVoltage[ii] = minVal + i*(maxVal-minVal)/divideBy2;
        } else if (i < (iSamples-divideBy2)) {
          m_OutputVoltage[ii] = maxVal - (i-divideBy2)*(maxVal-minVal)/((iSamples-divide)/2);
        } else {
          m_OutputVoltage[ii] = minVal - (iSamples-i)*(maxVal-minVal)/divideBy2;
        }
      }
      break;

    case SyncAcquisitionWaveformSawtooth:
    default:
      for (int ii=0; ii<iSamples; ii++) {
        int i = (ii+iSamples-shift) % iSamples;
        m_OutputVoltage[ii] = minVal + i*(maxVal-minVal)/iSamples;
      }
      break;
    }

    m_OutputVoltage[iSamples] = m_OutputVoltage[0]; // Return output voltage to starting value at the end of the waveform

    if (m_NIDAQPlugin) {
      m_NIDAQPlugin->setAnalogWaveform(chan, sampleRate, m_OutputVoltage.data(), iSamples+1);
    }
  }
}

//static QTime tick;

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int frameNumber)
{
  if (m_NIDAQPlugin) {
    m_NIDAQPlugin->pulseOutput();
  }

  if (m_SyncMode && m_AcquisitionParms) {
    if (m_Acquisition->acquisitionStatus(0.0) == 0) {
//      printf("QxrdSynchronizedAcquisition::acquiredFrameAvailable(%d)\n", frameNumber);

      int skipBefore = m_AcquisitionParms->skipBefore();
      int skipBetween = m_AcquisitionParms->skipBetween();
      int nPhases = m_AcquisitionParms->nphases();
      int nSummed = m_AcquisitionParms->nsummed();
      int nGroups = m_AcquisitionParms->postTrigger();
      int perGroup = nPhases*nSummed+skipBetween;
      int inGroup = (frameNumber-skipBefore) % perGroup;
      int phase = inGroup % nPhases;

      if (nPhases > 1) {
        if ((frameNumber >= skipBefore) && (frameNumber < (nGroups*perGroup-skipBetween+skipBefore))) {
          if (inGroup < nPhases*nSummed) {
            if (phase == 0) {
              if (m_NIDAQPlugin) {
//                printf("Triggered on frame %d\n", frameNumber);
                m_NIDAQPlugin->triggerAnalogWaveform();
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
  if (m_Acquisition && m_NIDAQPlugin) {
    QString fullChannel = get_SyncAcquisitionOutputChannel();

    m_Acquisition->printMessage(tr("Manually Setting %1 to %2 V")
                                .arg(fullChannel)
                                .arg(get_SyncAcquisitionManualValue()));

    m_NIDAQPlugin->setAnalogOutput(fullChannel, get_SyncAcquisitionManualValue());
  }
}

void QxrdSynchronizedAcquisition::triggerOnce()
{
  if (m_Acquisition && m_NIDAQPlugin) {
    QxrdAcquisitionParameterPack parms = m_Acquisition->acquisitionParameterPack();
    prepareForAcquisition(&parms);
    m_NIDAQPlugin->triggerAnalogWaveform();
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
