#include "qxrddistortioncorrection.h"
#include "qxrdmutexlocker.h"
#include "qxrdsettingssaver.h"
#include "levmar.h"
#include <math.h>

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
    m_IVals(saver, this, "iVals", QcepIntVector(), "Grid point i indices"),
    m_JVals(saver, this, "jVals", QcepIntVector(), "Grid point j indices"),
    m_XVals(saver, this, "xVals", QcepDoubleVector(), "Grid point x coords"),
    m_YVals(saver, this, "yVals", QcepDoubleVector(), "Grid point y coords"),
    m_FXVals(saver, this, "fxVals", QcepDoubleVector(), "Fitted x coords"),
    m_FYVals(saver, this, "fyVals", QcepDoubleVector(), "Fitted y coords"),
    m_DXVals(saver, this, "dxVals", QcepDoubleVector(), "Delta x coords"),
    m_DYVals(saver, this, "dyVals", QcepDoubleVector(), "Delta y coords"),
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
  prop_FXVals()->clear();
  prop_FYVals()->clear();
  prop_DXVals()->clear();
  prop_DYVals()->clear();
}

void QxrdDistortionCorrection::appendGridPoint(int i, int j, double x, double y)
{
  prop_IVals()->appendValue(i);
  prop_JVals()->appendValue(j);
  prop_XVals()->appendValue(x);
  prop_YVals()->appendValue(y);
  prop_FXVals()->appendValue(0);
  prop_FYVals()->appendValue(0);
  prop_DXVals()->appendValue(0);
  prop_DYVals()->appendValue(0);
}

void QxrdDistortionCorrection::evaluateFitGrid(double parms[], double hx[], int m, int n)
{
  double p0x = parms[0];
  double p0y = parms[1];
  double dxx = parms[2]-p0x;
  double dxy = parms[3]-p0y;
  double dyx = parms[4]-p0x;
  double dyy = parms[5]-p0y;

  for (int i=0; i<n; i++) {
    int ii = get_IVals()[i];
    int jj = get_JVals()[i];
    double fx = p0x + ii*dxx + jj*dyx;
    double fy = p0y + ii*dxy + jj*dyy;

    double dx = fx - get_XVals()[i];
    double dy = fy - get_YVals()[i];

    hx[i] = sqrt(dx*dx + dy*dy);
  }
}

static void fitGrid(double parms[], double hx[], int m, int n, void *adata)
{
  QxrdDistortionCorrection *dc = (QxrdDistortionCorrection*) adata;

  if (dc) {
    dc->evaluateFitGrid(parms, hx, m, n);
  }
}

void QxrdDistortionCorrection::fitCalibrationGrid()
{
  double parms[6];

  parms[0] = get_P0().x();
  parms[1] = get_P0().y();
  parms[2] = get_P1().x();
  parms[3] = get_P1().y();
  parms[4] = get_P2().x();
  parms[5] = get_P2().y();

  double info[LM_INFO_SZ];

  int ndata = get_IVals().count();

  int niter = dlevmar_dif(fitGrid, parms, NULL, 6, ndata, 300, NULL, info, NULL, NULL, this);

  if (niter >= 0) {
    set_F0(QPointF(parms[0], parms[1]));
    set_F1(QPointF(parms[2], parms[3]));
    set_F2(QPointF(parms[4], parms[5]));
  }
}
