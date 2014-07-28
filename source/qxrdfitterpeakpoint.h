#ifndef QXRDFITTERPEAKPOINT_H
#define QXRDFITTERPEAKPOINT_H

#include "qxrdfitter.h"

class QxrdFitterPeakPoint : public QxrdFitter
{
public:
  QxrdFitterPeakPoint(QxrdCenterFinder* cf, int index, double x0, double y0, double pkht, double bkgd);
  QxrdFitterPeakPoint();

  void fit();

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void evaluate(double *parm, double *xv, int np, int nx);
};

#endif // QXRDFITTERPEAKPOINT_H
