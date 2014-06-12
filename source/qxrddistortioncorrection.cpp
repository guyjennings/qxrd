#include "qxrddistortioncorrection.h"
#include "qxrdmutexlocker.h"
#include "qxrdsettingssaver.h"

QxrdDistortionCorrection::QxrdDistortionCorrection(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt)
  : QObject(),
    m_ObjectNamer(this, "distortion"),
    m_DistortionImagePath(saver, this, "distortionImagePath", "", "File path for distortion calibration image"),
    m_DistortionP0(saver, this, "distortionP0", QPointF(100,100), "Origin of distortion image grid"),
    m_DistortionP1(saver, this, "distortionP1", QPointF(200,100), "1st X Position on distortion grid"),
    m_DistortionP2(saver, this, "distortionP2", QPointF(100,200), "1st Y Position on distortion grid"),
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
