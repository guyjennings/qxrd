#include "qxrddistortioncorrection.h"
#include "qcepmutexlocker.h"
#include "qcepsettingssaver.h"
#include "levmar.h"
#include <qmath.h>
#include <stdio.h>
#include "qxrdexperiment.h"

QxrdDistortionCorrection::QxrdDistortionCorrection(QString name)
  : QcepObject(name),
    m_DistortionImagePath(this, "distortionImagePath", "", "File path for distortion calibration image"),
    m_P0(this, "p0", QPointF(100,100), "Origin of distortion image grid"),
    m_P1(this, "p1", QPointF(200,100), "1st X Position on distortion grid"),
    m_P2(this, "p2", QPointF(100,200), "1st Y Position on distortion grid"),
    m_N1(this, "n1", 40, "Number of grid points along x"),
    m_N2(this, "n2", 40, "Number of grid points along y"),
    m_F0(this, "f0", QPointF(100,100), "Fitted Origin of distortion image grid"),
    m_F1(this, "f1", QPointF(200,100), "Fitted 1st X Position on distortion grid"),
    m_F2(this, "f2", QPointF(100,200), "Fitted 1st Y Position on distortion grid"),
    m_IVals(this, "iVals", QcepIntVector(), "Grid point i indices"),
    m_JVals(this, "jVals", QcepIntVector(), "Grid point j indices"),
    m_XVals(this, "xVals", QcepDoubleVector(), "Grid point x coords"),
    m_YVals(this, "yVals", QcepDoubleVector(), "Grid point y coords"),
    m_FXVals(this, "fxVals", QcepDoubleVector(), "Fitted x coords"),
    m_FYVals(this, "fyVals", QcepDoubleVector(), "Fitted y coords"),
    m_DXVals(this, "dxVals", QcepDoubleVector(), "Delta x coords"),
    m_DYVals(this, "dyVals", QcepDoubleVector(), "Delta y coords"),
    m_WMin(this, "wMin", 1, "Minimum acceptable grid peak width (pixels)"),
    m_WMax(this, "wMax", 4, "Maximum acceptable grid peak width (pixels)"),
    m_WNom(this, "wNom", 2, "Nominal initial grid peak width (pixels)"),
    m_RatMin(this, "ratMin", 3, "Minimum acceptable peak/background ratio for grid peaks"),
    m_HgtMin(this, "hgtMin", 1000, "Minimum acceptable peak height for grid peaks"),
    m_DistMax(this, "distMax", QPointF(5,5), "Maximum acceptable grid peak position distance from nominal")
{
}

QxrdDistortionCorrectionPtr QxrdDistortionCorrection::newDistortionCorrection()
{
  QxrdDistortionCorrectionPtr dist(new QxrdDistortionCorrection("distortion"));

  return dist;
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
//  prop_FXVals()->appendValue(0);
//  prop_FYVals()->appendValue(0);
//  prop_DXVals()->appendValue(0);
//  prop_DYVals()->appendValue(0);
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

void QxrdDistortionCorrection::evalCalibrationGrid()
{
  double p0x = get_F0().x();
  double p0y = get_F0().y();
  double dxx = get_F1().x() - p0x;
  double dxy = get_F1().y() - p0y;
  double dyx = get_F2().x() - p0x;
  double dyy = get_F2().y() - p0y;

  int n = get_IVals().count();

  for (int i=0; i<n; i++) {
    int ii = get_IVals()[i];
    int jj = get_JVals()[i];
    double fx = p0x + ii*dxx + jj*dyx;
    double fy = p0y + ii*dxy + jj*dyy;

    double dx = fx - get_XVals()[i];
    double dy = fy - get_YVals()[i];

    prop_FXVals()->appendValue(fx);
    prop_FYVals()->appendValue(fy);
    prop_DXVals()->appendValue(dx);
    prop_DYVals()->appendValue(dy);
  }
}

void QxrdDistortionCorrection::dumpCalibrationGrid(QString path)
{
  int n = get_IVals().count();

  FILE *f = fopen(qPrintable(path), "a+");

  if (f) {
    fprintf(f, "i\tj\tx\ty\tfx\tfy\tdx\tdy\n");

    for (int i=0; i<n; i++) {
      fprintf(f, "%d\t%d\t%g\t%g\t%g\t%g\t%g\t%g\n",
                     get_IVals()[i], get_JVals()[i],
                     get_XVals()[i], get_YVals()[i],
                     get_FXVals()[i], get_FYVals()[i],
                     get_DXVals()[i], get_DYVals()[i]);
    }

    fclose(f);
  }
}
