#include "qcepfitterpeakorring.h"
#include "qcepcenterfinder.h"

QcepFitterPeakOrRing::QcepFitterPeakOrRing(QcepCenterFinder *cf, int index, double x0, double y0, double pkht, double bkgd) :
  QcepFitter(cf),
  m_Index(index),
  m_X0(x0),
  m_Y0(y0),
  m_Pkht(pkht),
  m_Bkgd(bkgd),
  m_FittedX(0.0),
  m_FittedY(0.0),
  m_FittedWidth(0.0),
  m_FittedHeight(0.0),
  m_FittedBkgd(0.0),
  m_FittedBkgdX(0.0),
  m_FittedBkgdY(0.0)
{
}

QcepFitterPeakOrRing::QcepFitterPeakOrRing() :
  QcepFitter(),
  m_Index(0),
  m_X0(0.0),
  m_Y0(0.0),
  m_Pkht(0.0),
  m_Bkgd(0.0),
  m_FittedX(0.0),
  m_FittedY(0.0),
  m_FittedWidth(0.0),
  m_FittedHeight(0.0),
  m_FittedBkgd(0.0),
  m_FittedBkgdX(0.0),
  m_FittedBkgdY(0.0)
{
}

double QcepFitterPeakOrRing::fittedR() const
{
  if (m_CenterFinder) {
    double dx = m_FittedX - m_CenterFinder->get_Center().x();
    double dy = m_FittedY - m_CenterFinder->get_Center().y();

    return sqrt(dx*dx + dy*dy);
  } else {
    return 0;
  }
}

double QcepFitterPeakOrRing::fittedAz() const
{
  if (m_CenterFinder) {
    double dx = m_FittedX - m_CenterFinder->get_Center().x();
    double dy = m_FittedY - m_CenterFinder->get_Center().y();

    return atan2(dy,dx);
  } else {
    return 0;
  }
}
