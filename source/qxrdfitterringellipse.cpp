#include "qxrdfitterringellipse.h"
#include "qxrdcenterfinder.h"

#include "levmar.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QxrdFitterRingEllipse::QxrdFitterRingEllipse(QxrdCenterFinder *cf, int ringIndex) :
  QxrdFitter(cf),
  m_RingIndex(ringIndex),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedA(0),
  m_FittedB(0),
  m_FittedRot(0)
{
}

QxrdFitterRingEllipse::QxrdFitterRingEllipse() :
  QxrdFitter(NULL),
  m_RingIndex(0),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedA(0),
  m_FittedB(0),
  m_FittedRot(0)
{
}

void QxrdFitterRingEllipse::staticEvaluate(double *p, double *hx, int m, int n, void *adata)
{
  QxrdFitterRingEllipse *rf = (QxrdFitterRingEllipse*) adata;

  if (rf) {
    rf->evaluate(p,hx,m,n);
  }
}

void QxrdFitterRingEllipse::evaluate(double *parm, double *xv, int np, int nx)
{
}

void QxrdFitterRingEllipse::fit()
{
  double parms[5];
  double info[LM_INFO_SZ];

  parms[0] = 0;
  parms[1] = 0;
  parms[2] = 100;
  parms[3] = 100;
  parms[4] = 0;

  int npts  = m_CenterFinder->countPowderRingPoints(m_RingIndex);

  int niter = dlevmar_dif(QxrdFitterRingEllipse::staticEvaluate,
                          parms, NULL, 5, npts,
                          m_CenterFinder->get_PeakFitIterations(),
                          NULL, info, NULL, NULL, this);

  if (niter > 0) {
    m_Reason    = Successful;
    m_FittedX   = parms[0];
    m_FittedY   = parms[1];
    m_FittedA   = parms[2];
    m_FittedB   = parms[3];
    m_FittedRot = parms[4];
  } else {
    m_Reason    = NoResult;
  }
}
