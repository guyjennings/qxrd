#include "qxrddistortioncorrection.h"
#include "qxrdmutexlocker.h"
#include "qxrdsettingssaver.h"

QxrdDistortionCorrection::QxrdDistortionCorrection(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt)
  : QObject(),
    m_ObjectNamer(this, "distortion"),
    m_DistortionImagePath(saver, this, "distortionImagePath", "", "File path for distortion calibration image"),
    m_P0(saver, this, "p0", QPointF(100,100), "Origin of distortion image grid"),
    m_P1(saver, this, "p1", QPointF(200,100), "1st X Position on distortion grid"),
    m_P2(saver, this, "p2", QPointF(100,200), "1st Y Position on distortion grid"),
    m_N1(saver, this, "n1", 40, "Number of grid points along x"),
    m_N2(saver, this, "n2", 40, "Number of grid points along y"),
    m_F0(saver, this, "f0", QPointF(100,100), "Fitted Origin of distortion image grid"),
    m_F1(saver, this, "f1", QPointF(200,100), "Fitted 1st X Position on distortion grid"),
    m_F2(saver, this, "f2", QPointF(100,200), "Fitted 1st Y Position on distortion grid"),
    m_IVals(saver, this, "iVals", QVector<int>(), "Grid point i indices"),
    m_JVals(saver, this, "jVals", QVector<int>(), "Grid point j indices"),
    m_XVals(saver, this, "xVals", QVector<double>(), "Grid point x coords"),
    m_YVals(saver, this, "yVals", QVector<double>(), "Grid point y coords"),
    m_WMin(saver, this, "wMin", 1, "Minimum acceptable grid peak width (pixels)"),
    m_WMax(saver, this, "wMax", 4, "Maximum acceptable grid peak width (pixels)"),
    m_WNom(saver, this, "wNom", 2, "Nominal initial grid peak width (pixels)"),
    m_RatMin(saver, this, "ratMin", 3, "Minimum acceptable peak/background ratio for grid peaks"),
    m_HgtMin(saver, this, "hgtMin", 1000, "Minimum acceptable peak height for grid peaks"),
    m_DistMax(saver, this, "distMax", QPointF(5,5), "Maximum acceptable grid peak position distance from nominal"),
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

void QxrdDistortionCorrection::clearGridPoints()
{
  prop_IVals()->clear();
  prop_JVals()->clear();
  prop_XVals()->clear();
  prop_YVals()->clear();
}

void QxrdDistortionCorrection::appendGridPoint(int i, int j, double x, double y)
{
  prop_IVals()->appendValue(i);
  prop_JVals()->appendValue(j);
  prop_XVals()->appendValue(x);
  prop_YVals()->appendValue(y);
}
