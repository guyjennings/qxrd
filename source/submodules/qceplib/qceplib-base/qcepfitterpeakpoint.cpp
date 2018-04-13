#include "qcepfitterpeakpoint.h"
#include "qcepcenterfinder.h"

#include "levmar.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QcepFitterPeakPoint::QcepFitterPeakPoint(QcepCenterFinder *cf, int index, double x0, double y0, double pkht, double bkgd) :
  QcepFitterPeakOrRing(cf, index, x0, y0, pkht, bkgd)
{
}

QcepFitterPeakPoint::QcepFitterPeakPoint() :
  QcepFitterPeakOrRing()
{
}

void QcepFitterPeakPoint::staticEvaluate(double *p, double *hx, int m, int n, void *adata)
{
  QcepFitterPeakPoint *rf = (QcepFitterPeakPoint*) adata;

  if (rf) {
    rf->evaluate(p,hx,m,n);
  }
}

void QcepFitterPeakPoint::evaluate(double *parm, double *xv, int np, int /*nx*/)
{
  if (m_CenterFinder) {
//    m_CenterFinder->printMessage("Fitting");

    if (np!=7) {
      m_CenterFinder->printMessage("Wrong number of parameters");
    } else {
      double cx = parm[0];
      double cy = parm[1];
      double r  = parm[2];
      double ht = parm[3];
      double bg = parm[4];
      double bx = parm[5];
      double by = parm[6];

      double cx0 = m_X0;
      double cy0 = m_Y0;
      double rr0 = m_CenterFinder->get_PeakFitRadius();

      int n = rr0*2+1;
      int x0 = cx0 - rr0;
      int y0 = cy0 - rr0;
//      int nn = n*n;
      int i=0;

      for (int y=y0; y<y0+n; y++) {
        for (int x=x0; x<x0+n; x++) {
          double d = m_CenterFinder->imageValue(x,y);

          double dx = x-cx;
          double dy = y-cy;
          double pk = bg + dx*bx + dy*by + ht*exp(-((dx*dx+dy*dy)/(2.0*r*r)));

          xv[i++] = pk - d;
        }
      }
    }
  }
}

int QcepFitterPeakPoint::fit()
{
  int niter = -1;

  if (m_CenterFinder) {

    double x = m_X0, y = m_Y0;

//    if (qcepDebug(DEBUG_FITTING) || m_CenterFinder->get_PeakFitDebug()) {
//      m_CenterFinder -> printMessage(QObject::tr("Fitting i: %1, x0: %2, y0: %3")
//                                     .arg(index()).arg(m_X0).arg(m_Y0));
//    }

    int    width = 0, height = 0;

    QcepDoubleImageDataPtr data(m_CenterFinder->data());

    if (data) {
      width  = data -> get_Width()+1;
      height = data -> get_Height()+1;

      if (x<0 || x>width || y<0 || y>height) {
        m_Reason = OutsideData;
      } else {
        double dr = m_CenterFinder->get_PeakFitRadius();

        double bkgd = ( m_CenterFinder->imageValue(x+dr, y) +
                        m_CenterFinder->imageValue(x, y+dr) +
                        m_CenterFinder->imageValue(x-dr, y) +
                        m_CenterFinder->imageValue(x, y-dr) )/4.0;
        double pkht = m_CenterFinder->imageValue(x,y) - bkgd;

        double parms[7];

        parms[0] = x;
        parms[1] = y;
        parms[2] = 2.0;
        parms[3] = pkht;
        parms[4] = bkgd;
        parms[5] = 0;
        parms[6] = 0;

        double info[LM_INFO_SZ];

        int n = m_CenterFinder->get_PeakFitRadius()*2+1;

        niter = dlevmar_dif(QcepFitterPeakPoint::staticEvaluate,
                            parms, NULL, 7, n*n,
                            m_CenterFinder->get_PeakFitIterations(),
                            NULL, info, NULL, NULL, this);

        if (niter > 0) {
          m_Reason       = Successful;
          m_FittedX      = parms[0];
          m_FittedY      = parms[1];
          m_FittedWidth  = parms[2];
          m_FittedHeight = parms[3];
          m_FittedBkgd   = parms[4];
          m_FittedBkgdX  = parms[5];
          m_FittedBkgdY  = parms[6];

          double dx = m_FittedX - m_X0;
          double dy = m_FittedY - m_Y0;
          double dr = sqrt(dx*dx + dy*dy);

          if ((fabs(m_FittedWidth)<m_CenterFinder->get_FittedWidthMin()) ||
              (fabs(m_FittedWidth)>m_CenterFinder->get_FittedWidthMax())) {
            m_Reason = BadWidth;
          } else if ((m_FittedHeight/m_Pkht)<m_CenterFinder->get_FittedHeightMinRatio()) {
            m_Reason = BadHeight;
          } else if (dr>m_CenterFinder->get_FittedPositionMaxDistance()) {
            m_Reason = BadPosition;
          }
        }
      }
    }
  }

  return niter;
}

