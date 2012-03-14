#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionWPtr acq) :
  QObject(acq.data()),
  m_ExtraInputs(saver, this, "extraInputs", 0),
  m_ChannelNames(saver, this, "channelNames", QStringList()),
  m_SampleRate(saver, this, "sampleRate", 1000.0),
  m_StartOffset(saver, this, "startOffset", 0.0),
  m_EndOffset(saver, this, "endOffset", 0.0),
  m_AcquireDelay(saver, this, "acquireDelay", 0.107),
  m_Experiment(doc),
  m_Acquisition(acq)
{
}

void QxrdAcquisitionExtraInputs::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionExtraInputs::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

