#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionWPtr acq) :
  QObject(acq.data()),
  m_ExtraInputs(saver, this, "extraInputs", 8),
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

