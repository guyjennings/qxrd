#ifndef QXRDFITTERRINGCIRCLE_H
#define QXRDFITTERRINGCIRCLE_H

#include "qxrdlib_global.h"
#include "qxrdfitter.h"

class QXRD_EXPORT QxrdFitterRingCircle : public QxrdFitter
{
public:
  QxrdFitterRingCircle(QxrdCenterFinder *cf, int ringIndex, double x0, double y0);
  QxrdFitterRingCircle();

  int fit();

  int      ringIndex() const { return m_RingIndex; }
  double   x0() const { return m_X0; }
  double   y0() const { return m_Y0; }
  double   fittedX() const { return m_FittedX; }
  double   fittedY() const { return m_FittedY; }
  double   fittedR() const { return m_FittedR; }

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void        evaluate(double *parm, double *xv, int np, int nx);

protected:
  int      m_RingIndex;
  double   m_X0;
  double   m_Y0;
  double   m_FittedX;
  double   m_FittedY;
  double   m_FittedR;
};

#endif // QXRDFITTERRINGCIRCLE_H
