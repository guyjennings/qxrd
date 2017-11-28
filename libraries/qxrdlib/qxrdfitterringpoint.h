#ifndef QXRDFITTERRINGPOINT_H
#define QXRDFITTERRINGPOINT_H

#include "qxrdlib_global.h"
#include "qxrdfitterpeakorring.h"

class QXRD_EXPORT QxrdFitterRingPoint : public QxrdFitterPeakOrRing
{
public:
  QxrdFitterRingPoint(QxrdCenterFinder* cf, int index, double x0, double y0, double pkht, double bkgd);
  QxrdFitterRingPoint();

  virtual ~QxrdFitterRingPoint();

  int fit();

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void        evaluate(double *parm, double *xv, int np, int nx);
};

#endif // QXRDFITTERRINGPOINT_H
