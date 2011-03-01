#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"
#include "qwt_math.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdAcquisition *acq) :
  m_SyncAcquisitionMode(this,"syncAcquisitionMode", 0),
  m_SyncAcquisitionWaveform(this,"syncAcquisitionWaveform", 0),
  m_SyncAcquisitionOutputChannel(this,"syncAcquisitionOutputChannel", 0),
  m_SyncAcquisitionFlagChannel(this,"syncAcquisitionFlagChannel", 0),
  m_SyncAcquisitionMinimum(this,"syncAcquisitionMinimum", 0.0),
  m_SyncAcquisitionMaximum(this,"syncAcquisitionMaximum", 5.0),
  m_SyncAcquisitionSymmetry(this,"syncAcquisitionSymmetry", 0.0),
  m_Acquisition(acq),
  m_NIDAQPlugin(NULL),
  m_SyncMode(0),
  QObject(NULL)
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

void QxrdSynchronizedAcquisition::prepareForAcquisition()
{
  m_SyncMode = 0;

  if (!m_Acquisition->get_AcquireDark()) {
    double exposureTime = m_Acquisition->get_ExposureTime();
    //  int    nsummed      = m_Acquisition->get_SummedExposures();
    int    nphases      = m_Acquisition->get_FilesInGroup();
    double cycleTime    = exposureTime/**nsummed*/*nphases;
    double sampleRate   = 1000;
    double nSamples     = cycleTime*sampleRate;
    double minVal       = get_SyncAcquisitionMinimum();
    double maxVal       = get_SyncAcquisitionMaximum();
    int chan            = get_SyncAcquisitionOutputChannel();
    int wfm             = get_SyncAcquisitionWaveform();
    m_SyncMode          = get_SyncAcquisitionMode();
    int symm            = get_SyncAcquisitionSymmetry();

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
            x = M_PI*(i-divide)/(iSamples-divide);
          }
          m_OutputVoltage[i] = minVal + (maxVal-minVal)*(sin(x)-1.0)/2.0;
        }
        break;

      case SyncAcquisitionWaveformTriangle:
        for (int i=0; i<iSamples; i++) {
          if (i<divide) {
            m_OutputVoltage[i] = minVal + i*(maxVal-minVal)/divide;
          } else {
            m_OutputVoltage[i] = maxVal + (i-divide)*(maxVal-minVal)/(iSamples-divide);
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
        m_NIDAQPlugin->setAnalogChannel(chan-1);
        m_NIDAQPlugin->setAnalogWaveform(sampleRate, m_OutputVoltage.data(), iSamples+1);
      }
    }
  }
}

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int currentPhase)
{
  if (m_SyncMode) {
    if (m_Acquisition->acquisitionStatus(0.0) == 0) {
      if (currentPhase == 0) {
        if (m_NIDAQPlugin) {
          m_NIDAQPlugin->triggerAnalogWaveform();
        }
      }
    }
  }
  //  printf("Sync acquired frame available %d, %d\n", currentExposure, currentFile);

//  if (m_NIDAQPlugin) {
//    m_NIDAQPlugin->setAnalogOut(0, m_OutputVoltage[currentPhase]);
//  }
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
