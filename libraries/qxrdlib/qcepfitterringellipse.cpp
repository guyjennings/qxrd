#include "qcepfitterringellipse.h"
#include "qcepcenterfinder.h"

#include "levmar.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QcepFitterRingEllipse::QcepFitterRingEllipse(QcepCenterFinder *cf, int ringIndex, double x0, double y0) :
  QcepFitter(cf),
  m_RingIndex(ringIndex),
  m_X0(x0),
  m_Y0(y0),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedA(0),
  m_FittedB(0),
  m_FittedRot(0)
{
}

QcepFitterRingEllipse::QcepFitterRingEllipse() :
  QcepFitter(NULL),
  m_RingIndex(0),
  m_X0(0),
  m_Y0(0),
  m_FittedX(0),
  m_FittedY(0),
  m_FittedA(0),
  m_FittedB(0),
  m_FittedRot(0)
{
}

QcepFitterRingEllipse::~QcepFitterRingEllipse()
{
}

void QcepFitterRingEllipse::staticEvaluate(double *p, double *hx, int m, int n, void *adata)
{
  QcepFitterRingEllipse *rf = (QcepFitterRingEllipse*) adata;

  if (rf) {
    rf->evaluate(p,hx,m,n);
  }
}

void QcepFitterRingEllipse::evaluate(double *parm, double *xv, int /*np*/, int nx)
{
  if (m_CenterFinder) {
    double cx = parm[0];
    double cy = parm[1];
    double a  = parm[2];
    double b  = parm[3];
    double az = parm[4];

    double cosaz = cos(az);
    double sinaz = sin(az);

    for (int i=0; i<nx; i++) {
      QcepPowderPoint pt = m_CenterFinder->powderRingPoint(m_RingIndex, i);

      double ptaz = atan2(pt.y() - cy, pt.x() - cx) - az;

      double x = cx + a*cos(ptaz);
      double y = cy + b*sin(ptaz);

      double xp = x*cosaz - y*sinaz;
      double yp = x*sinaz + y*cosaz;

      double dx = xp - pt.x();
      double dy = yp - pt.y();

      xv[i] = sqrt(dx*dx + dy*dy);
    }
  }
}

int QcepFitterRingEllipse::fit()
{
  int niter = -1;

  if (m_CenterFinder) {
    int npts  = m_CenterFinder->countPowderRingPoints(m_RingIndex);
    double rsum = 0;

    for (int i=0; i<npts; i++) {
      QcepPowderPoint pt = m_CenterFinder->powderRingPoint(m_RingIndex, i);

      double dx = pt.x() - m_X0;
      double dy = pt.y() - m_Y0;

      rsum += sqrt(dx*dx + dy*dy);
    }

    double parms[5];
    double info[LM_INFO_SZ];

    parms[0] = m_X0;
    parms[1] = m_Y0;
    parms[2] = rsum/npts;
    parms[3] = rsum/npts;
    parms[4] = 0;

    niter = dlevmar_dif(QcepFitterRingEllipse::staticEvaluate,
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

  return niter;
}
