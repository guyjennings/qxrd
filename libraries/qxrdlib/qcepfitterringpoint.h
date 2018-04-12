#ifndef QCEPFITTERRINGPOINT_H
#define QCEPFITTERRINGPOINT_H

#include "qceplib_global.h"
#include "qcepfitterpeakorring.h"

class QCEP_EXPORT QcepFitterRingPoint : public QcepFitterPeakOrRing
{
public:
  QcepFitterRingPoint(QcepCenterFinder* cf, int index, double x0, double y0, double pkht, double bkgd);
  QcepFitterRingPoint();

  virtual ~QcepFitterRingPoint();

  int fit();

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void        evaluate(double *parm, double *xv, int np, int nx);
};

#endif // QCEPFITTERRINGPOINT_H
