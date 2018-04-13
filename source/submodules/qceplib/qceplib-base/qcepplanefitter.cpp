#include "qcepplanefitter.h"

QcepPlaneFitter::QcepPlaneFitter() :
  s(0),
  sx(0),
  sy(0),
  sxx(0),
  syy(0),
  sxy(0),
  sz(0),
  sxz(0),
  syz(0)
{
}

void QcepPlaneFitter::addPoint(double x, double y, double z)
{
  s   += 1;
  sx  += x;
  sy  += y;
  sxx += x*x;
  syy += y*y;
  sxy += x*y;
  sz  += z;
  sxz += x*z;
  syz += y*z;
}

double QcepPlaneFitter::value(double x, double y)
{
  double a = -(sx*(sy*syz-syy*sz)+sxy*(sy*sz-s*syz)+sxz*(s*syy-sy*sy))/(sxx*(sy*sy-s*syy)+sx*sx*syy-2*sx*sxy*sy+s*sxy*sxy);
  double b =  (sxx*(sy*sz-s*syz)-sx*sxy*sz+sx*sx*syz+sxz*(s*sxy-sx*sy))/(sxx*(sy*sy-s*syy)+sx*sx*syy-2*sx*sxy*sy+s*sxy*sxy);
  double c = -(sxx*(syy*sz-sy*syz)-sxy*sxy*sz+sx*sxy*syz+sxz*(sxy*sy-sx*syy))/(sxx*(sy*sy-s*syy)+sx*sx*syy-2*sx*sxy*sy+s*sxy*sxy);

  return a*x + b*y + c;
}
