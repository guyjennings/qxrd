#include "qxrdfitterpeakorring.h"

QxrdFitterPeakOrRing::QxrdFitterPeakOrRing(QxrdCenterFinder *cf, int index, double x0, double y0, double pkht, double bkgd) :
  QxrdFitter(cf),
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

QxrdFitterPeakOrRing::QxrdFitterPeakOrRing() :
  QxrdFitter(),
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
