#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisition.h"

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
  double exposureTime = m_Acquisition->get_ExposureTime();
//  int    nsummed      = m_Acquisition->get_SummedExposures();
  int    nphases      = m_Acquisition->get_FilesInGroup();
  double cycleTime    = exposureTime/**nsummed*/*nphases;
  double sampleRate   = 1000;
  double nSamples     = cycleTime*sampleRate;
  double minVal       = get_SyncAcquisitionMinimum();
  double maxVal       = get_SyncAcquisitionMaximum();
  int chan            = get_SyncAcquisitionOutputChannel();

  while (nSamples > 10000) {
    sampleRate /= 10;
    nSamples = cycleTime*sampleRate;
  }

  int iSamples = (int) nSamples;

  m_OutputVoltage.resize(iSamples);

  for (int i=0; i<iSamples; i++) {
    m_OutputVoltage[i] = minVal + i*(maxVal-minVal)/iSamples;
  }

  if (m_NIDAQPlugin) {
    m_NIDAQPlugin->setAnalogChannel(chan-1);
    m_NIDAQPlugin->setAnalogWaveform(sampleRate, m_OutputVoltage.data(), iSamples);
  }
}

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int currentPhase)
{
  if (m_Acquisition->acquisitionStatus(0.0) == 0) {
    if (currentPhase == 0) {
      if (m_NIDAQPlugin) {
        m_NIDAQPlugin->triggerAnalogWaveform();
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
