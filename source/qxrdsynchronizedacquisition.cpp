#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"
#include "qwt_math.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdAcquisition *acq) :
  QObject(NULL),
  m_SyncAcquisitionMode(this,"syncAcquisitionMode", 0),
  m_SyncAcquisitionWaveform(this,"syncAcquisitionWaveform", 0),
  m_SyncAcquisitionOutputChannel(this,"syncAcquisitionOutputChannel", 0),
  m_SyncAcquisitionFlagChannel(this,"syncAcquisitionFlagChannel", 0),
  m_SyncAcquisitionMinimum(this,"syncAcquisitionMinimum", 0.0),
  m_SyncAcquisitionMaximum(this,"syncAcquisitionMaximum", 5.0),
  m_SyncAcquisitionSymmetry(this,"syncAcquisitionSymmetry", 0.0),
  m_Acquisition(acq),
  m_AcquisitionParms(NULL),
  m_NIDAQPlugin(NULL),
  m_SyncMode(0)
{
}

void QxrdSynchronizedAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdNIDAQPluginInterface *QxrdSynchronizedAcquisition::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

void QxrdSynchronizedAcquisition::finishedAcquisition()
{
  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::prepareForDarkAcquisition(QxrdAcquisition::QxrdDarkAcquisitionParameterPack *parms)
{
  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::prepareForAcquisition(QxrdAcquisition::QxrdAcquisitionParameterPack *parms)
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
  int chan            = get_SyncAcquisitionOutputChannel();
  int wfm             = get_SyncAcquisitionWaveform();
  m_SyncMode          = get_SyncAcquisitionMode();
  double symm         = get_SyncAcquisitionSymmetry();

  if (symm > 1.0) {
    symm = 1.0;
  } else if (symm < -1.0) {
    symm = -1.0;
  }

  if (m_SyncMode) {
    while (nSamples > 10000) {
      sampleRate /= 10;
      nSamples = cycleTime*sampleRate;
    }

    int iSamples = (int) nSamples;
    double divide = iSamples * (0.5 + symm/2.0);

    m_OutputVoltage.resize(iSamples+1);

    switch (wfm) {
    case SyncAcquisitionWaveformSquare:
      for (int i=0; i<iSamples; i++) {
        if (i<divide) {
          m_OutputVoltage[i] = minVal;
        } else {
          m_OutputVoltage[i] = maxVal;
        }
      }
      break;

      case SyncAcquisitionWaveformSine:
      for (int i=0; i<iSamples; i++) {
        double x;
        if (i<divide) {
          x = M_PI*i/divide;
        } else {
          x = M_PI+M_PI*(i-divide)/(iSamples-divide);
        }
        m_OutputVoltage[i] = minVal + (maxVal-minVal)*(1.0 - cos(x))/2.0;
      }
      break;

      case SyncAcquisitionWaveformTriangle:
      for (int i=0; i<iSamples; i++) {
        if (i<divide) {
          m_OutputVoltage[i] = minVal + i*(maxVal-minVal)/divide;
        } else {
          m_OutputVoltage[i] = maxVal - (i-divide)*(maxVal-minVal)/(iSamples-divide);
        }
      }
      break;

      case SyncAcquisitionWaveformSawtooth:
      default:
      for (int i=0; i<iSamples; i++) {
        m_OutputVoltage[i] = minVal + i*(maxVal-minVal)/iSamples;
      }
      break;
    }

    m_OutputVoltage[iSamples] = minVal; // Return output voltage to starting value at the end of the waveform

    if (m_NIDAQPlugin) {
      m_NIDAQPlugin->setAnalogWaveform(chan-1, sampleRate, m_OutputVoltage.data(), iSamples+1);
    }
  }
}

//static QTime tick;

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int frameNumber)
{
  if (m_SyncMode && m_AcquisitionParms) {
    if (m_Acquisition->acquisitionStatus(0.0) == 0) {
      int skipBefore = m_AcquisitionParms->skipBefore();
      int skipBetween = m_AcquisitionParms->skipBetween();
      int nPhases = m_AcquisitionParms->nphases();
      int nSummed = m_AcquisitionParms->nsummed();
      int nGroups = m_AcquisitionParms->postTrigger();
      int perGroup = nPhases*nSummed+skipBetween;
      int inGroup = (frameNumber-skipBefore) % perGroup;
      int phase = inGroup % nPhases;

      if ((frameNumber >= skipBefore) && (frameNumber < (nGroups*perGroup-skipBetween+skipBefore))) {
        if (inGroup < nPhases*nSummed) {
          if (phase == 0) {
            if (m_NIDAQPlugin) {
              m_NIDAQPlugin->triggerAnalogWaveform();
            }
          }
        }
      }
//      printf("elapsed[%d] %d msec\n", currentPhase, tick.restart());
    }
  }
}

void QxrdSynchronizedAcquisition::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdSynchronizedAcquisition::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}
