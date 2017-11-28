#ifndef QXRDFITTERRINGELLIPSE_H
#define QXRDFITTERRINGELLIPSE_H

#include "qxrdlib_global.h"
#include "qxrdfitter.h"

class QXRD_EXPORT QxrdFitterRingEllipse : public QxrdFitter
{
public:
  QxrdFitterRingEllipse(QxrdCenterFinder *cf, int ringIndex, double x0, double y0);
  QxrdFitterRingEllipse();

  virtual ~QxrdFitterRingEllipse();

  int fit();

  int      ringIndex() const { return m_RingIndex; }
  double   x0() const { return m_X0; }
  double   y0() const { return m_Y0; }
  double   fittedX() const { return m_FittedX; }
  double   fittedY() const { return m_FittedY; }
  double   fittedA() const { return m_FittedA; }
  double   fittedB() const { return m_FittedB; }
  double   fittedRot() const { return m_FittedRot; }

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void        evaluate(double *parm, double *xv, int np, int nx);

protected:
  int      m_RingIndex;
  double   m_X0;
  double   m_Y0;
  double   m_FittedX;
  double   m_FittedY;
  double   m_FittedA;
  double   m_FittedB;
  double   m_FittedRot;
};

#endif // QXRDFITTERRINGELLIPSE_H
