#ifndef QCEPPLANEFITTER_H
#define QCEPPLANEFITTER_H

#include "qceplib_global.h"

class QCEP_EXPORT QcepPlaneFitter
{
public:
  QcepPlaneFitter();

  void   addPoint(double x, double y, double z);
  double value(double x, double y);

private:
  double s;
  double sx;
  double sy;
  double sxx;
  double syy;
  double sxy;
  double sz;
  double sxz;
  double syz;
};

#endif // QCEPPLANEFITTER_H
