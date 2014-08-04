#ifndef QXRDFITTERRINGCIRCLE_H
#define QXRDFITTERRINGCIRCLE_H

#include "qxrdfitter.h"

class QxrdFitterRingCircle : public QxrdFitter
{
public:
  QxrdFitterRingCircle(QxrdCenterFinder *cf, int ringIndex);
  QxrdFitterRingCircle();

  void fit();

  int      ringIndex() const { return m_RingIndex; }
  double   fittedX() const { return m_FittedX; }
  double   fittedY() const { return m_FittedY; }
  double   fittedR() const { return m_FittedR; }

private:
  static void staticEvaluate(double *parm, double *xv, int np, int nx, void *adata);
  void        evaluate(double *parm, double *xv, int np, int nx);

protected:
  int      m_RingIndex;
  double   m_FittedX;
  double   m_FittedY;
  double   m_FittedR;
};

#endif // QXRDFITTERRINGCIRCLE_H
