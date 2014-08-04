#include "qxrdfitterringcircle.h"
#include "qxrdcenterfinder.h"

#include "levmar.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QxrdFitterRingCircle::QxrdFitterRingCircle(QxrdCenterFinder *cf, int ringIndex) :
  QxrdFitter(cf),
  m_RingIndex(ringIndex),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedR(0)
{
}

QxrdFitterRingCircle::QxrdFitterRingCircle() :
  QxrdFitter(),
  m_RingIndex(-1),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedR(0)
{
}

void QxrdFitterRingCircle::staticEvaluate(double *p, double *hx, int m, int n, void *adata)
{
  QxrdFitterRingCircle *rf = (QxrdFitterRingCircle*) adata;

  if (rf) {
    rf->evaluate(p,hx,m,n);
  }
}

void QxrdFitterRingCircle::evaluate(double *parm, double *xv, int np, int nx)
{
}

void QxrdFitterRingCircle::fit()
{
  double parms[3];
  double info[LM_INFO_SZ];

  parms[0] = 0;
  parms[1] = 0;
  parms[2] = 100;

  int npts  = m_CenterFinder->countPowderRingPoints(m_RingIndex);

  int niter = dlevmar_dif(QxrdFitterRingCircle::staticEvaluate,
                          parms, NULL, 3, npts,
                          m_CenterFinder->get_PeakFitIterations(),
                          NULL, info, NULL, NULL, this);

  if (niter > 0) {
    m_Reason   = Successful;

    m_FittedX  = parms[0];
    m_FittedY  = parms[1];
    m_FittedR  = parms[2];
  } else {
    m_Reason   = NoResult;
  }
}
