#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdmutexlocker.h"
#include "qxrdacquisitionparameters.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdAcquisitionParameters *acq) :
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

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int currentPhase)
{
//  printf("Sync acquired frame available %d, %d\n", currentExposure, currentFile);

//  if (m_NIDAQPlugin) {
//    m_NIDAQPlugin->setAnalogOut(0, m_OutputVoltage[currentPhase]);
//  }
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
