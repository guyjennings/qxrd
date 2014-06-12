#include "qxrddistortioncorrection.h"
#include "qxrdmutexlocker.h"

QxrdDistortionCorrection::QxrdDistortionCorrection(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt)
  : QObject(),
    m_ObjectNamer(this, "distortion"),
    m_Experiment(expt)
{
}

void QxrdDistortionCorrection::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdDistortionCorrection::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

QPointF QxrdDistortionCorrection::transform(QPointF pt)
{
  return pt;
}

QPointF QxrdDistortionCorrection::inverse(QPointF pt)
{
  return pt;
}
