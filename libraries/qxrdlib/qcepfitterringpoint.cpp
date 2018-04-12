#include "qcepfitterringpoint.h"
#include "qcepcenterfinder.h"

#include "levmar.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QcepFitterRingPoint::QcepFitterRingPoint(QcepCenterFinder *cf, int index, double x0, double y0, double pkht, double bkgd) :
  QcepFitterPeakOrRing(cf, index, x0, y0, pkht, bkgd)
{
}

QcepFitterRingPoint::QcepFitterRingPoint() :
  QcepFitterPeakOrRing()
{
}

QcepFitterRingPoint::~QcepFitterRingPoint()
{
}

void QcepFitterRingPoint::staticEvaluate(double *p, double *hx, int m, int n, void *adata)
{
  QcepFitterRingPoint *rf = (QcepFitterRingPoint*) adata;

  if (rf) {
    rf->evaluate(p,hx,m,n);
  }
}

void QcepFitterRingPoint::evaluate(double *parm, double *xv, int np, int /*nx*/)
{
  if (m_CenterFinder) {
//    m_CenterFinder->printMessage("Fitting");

    if (np!=6) {
      m_CenterFinder->printMessage("Wrong number of parameters");
    } else {
      double pr = parm[0];
      double wd = parm[1];
      double ht = parm[2];
      double bg = parm[3];
      double bx = parm[4];
      double by = parm[5];

      double dr = m_CenterFinder->get_PeakFitRadius();
      double xc = m_CenterFinder->get_Center().x();
      double yc = m_CenterFinder->get_Center().y();
      double cx0 = m_X0;
      double cy0 = m_Y0;
      double az = atan2(cy0-yc, cx0-xc);

      int n = dr*2+1;
      int x0 = cx0 - dr;
      int y0 = cy0 - dr;
//      int nn = n*n;
      int i=0;

      double cx  = xc + pr*cos(az);
      double cy  = yc + pr*sin(az);

      double dx0 = cx - xc;
      double dy0 = cy - yc;
      double r0  = sqrt(dx0*dx0+dy0*dy0);

      for (int y=y0; y<y0+n; y++) {
        for (int x=x0; x<x0+n; x++) {
          double d = m_CenterFinder->imageValue(x,y);

          double dx = x-cx;
          double dy = y-cy;
          double dx1 = x - xc;
          double dy1 = y - yc;
          double r1 = sqrt(dx1*dx1+dy1*dy1);
          double dr = r1-r0;
          double pk = bg + dx*bx + dy*by + ht*exp(-((dr*dr)/(2.0*wd*wd)));

          xv[i++] = pk - d;
        }
      }
    }
  }
}

int QcepFitterRingPoint::fit()
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
        double xc = m_CenterFinder->get_Center().x();
        double yc = m_CenterFinder->get_Center().y();
        double dx = x - xc;
        double dy = y - yc;
        double r  = sqrt(dx*dx + dy*dy);
        double chi = atan2(dy, dx);
        double dr  = m_CenterFinder->get_PeakFitRadius();
        double bkgd = ( m_CenterFinder->imageValue(xc+(r+dr)*cos(chi), yc+(r+dr)*sin(chi))
                       +m_CenterFinder->imageValue(xc+(r-dr)*cos(chi), yc+(r-dr)*sin(chi)))/2.0;
        double pkht = m_CenterFinder->imageValue(x,y) - bkgd;

        double parms[6];

        parms[0] = r;
        parms[1] = 2.0;
        parms[2] = pkht;
        parms[3] = bkgd;
        parms[4] = 0;
        parms[5] = 0;

        double info[LM_INFO_SZ];

        int n = m_CenterFinder->get_PeakFitRadius()*2+1;

        niter = dlevmar_dif(QcepFitterRingPoint::staticEvaluate,
                                parms, NULL, 6, n*n,
                                m_CenterFinder->get_PeakFitIterations(),
                                NULL, info, NULL, NULL, this);

        if (niter > 0) {
          m_Reason       = Successful;
          m_FittedX      = xc + parms[0]*cos(chi);
          m_FittedY      = yc + parms[0]*sin(chi);
          m_FittedWidth  = parms[1];
          m_FittedHeight = parms[2];
          m_FittedBkgd   = parms[3];
          m_FittedBkgdX  = parms[4];
          m_FittedBkgdY  = parms[5];

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

