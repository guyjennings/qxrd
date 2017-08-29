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

QxrdFitterRingCircle::QxrdFitterRingCircle(QxrdCenterFinder *cf, int ringIndex, double x0, double y0) :
  QxrdFitter(cf),
  m_RingIndex(ringIndex),
  m_X0(x0),
  m_Y0(y0),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedR(0)
{
}

QxrdFitterRingCircle::QxrdFitterRingCircle() :
  QxrdFitter(),
  m_RingIndex(-1),
  m_X0(0),
  m_Y0(0),
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

void QxrdFitterRingCircle::evaluate(double *parm, double *xv, int /*np*/, int nx)
{
  if (m_CenterFinder) {
    double cx = parm[0];
    double cy = parm[1];
    double r  = parm[2];

    for (int i=0; i<nx; i++) {
      QxrdPowderPoint pt = m_CenterFinder->powderRingPoint(m_RingIndex, i);

      if (pt.isValid()) {
        double dx = pt.x() - cx;
        double dy = pt.y() - cy;
        double rcalc = sqrt(dx*dx + dy*dy);

        xv[i] = rcalc - r;
      }
    }
  }
}

int QxrdFitterRingCircle::fit()
{
  int niter = -1;

  if (m_CenterFinder) {

    int npts  = m_CenterFinder->countPowderRingPoints(m_RingIndex);
    double rsum = 0;

    for (int i=0; i<npts; i++) {
      QxrdPowderPoint pt = m_CenterFinder->powderRingPoint(m_RingIndex, i);

      double dx = pt.x() - m_X0;
      double dy = pt.y() - m_Y0;

      rsum += sqrt(dx*dx + dy*dy);
    }

    double parms[3];
    double info[LM_INFO_SZ];

    parms[0] = m_X0;
    parms[1] = m_Y0;
    parms[2] = rsum/npts;

    niter = dlevmar_dif(QxrdFitterRingCircle::staticEvaluate,
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

  return niter;
}
