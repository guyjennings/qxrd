#ifndef QCEPFITTERPEAKPOINT_H
#define QCEPFITTERPEAKPOINT_H

#include "qceplib_global.h"
#include "qcepfitterpeakorring.h"

class QCEP_EXPORT QcepFitterPeakPoint : public QcepFitterPeakOrRing
{
public:
  QcepFitterPeakPoint(QcepCenterFinder* cf, int index, double x0, double y0, double pkht, double bkgd);
  QcepFitterPeakPoint();

  int fit();

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void evaluate(double *parm, double *xv, int np, int nx);
};

#endif // QCEPFITTERPEAKPOINT_H
