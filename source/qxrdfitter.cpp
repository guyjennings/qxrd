#include "qxrdfitter.h"

QxrdFitter::QxrdFitter(QxrdCenterFinder *cf, int index, double x0, double y0, double pkht, double bkgd) :
  m_CenterFinder(cf),
  m_Index(index),
  m_X0(x0),
  m_Y0(y0),
  m_Pkht(pkht),
  m_Bkgd(bkgd),
  m_Reason(NoResult),
  m_FittedX(0.0),
  m_FittedY(0.0),
  m_FittedWidth(0.0),
  m_FittedHeight(0.0),
  m_FittedBkgd(0.0),
  m_FittedBkgdX(0.0),
  m_FittedBkgdY(0.0)
{
}

QxrdFitter::QxrdFitter() :
  m_CenterFinder(NULL),
  m_Index(0),
  m_X0(0.0),
  m_Y0(0.0),
  m_Pkht(0.0),
  m_Bkgd(0.0),
  m_Reason(NoResult),
  m_FittedX(0.0),
  m_FittedY(0.0),
  m_FittedWidth(0.0),
  m_FittedHeight(0.0),
  m_FittedBkgd(0.0),
  m_FittedBkgdX(0.0),
  m_FittedBkgdY(0.0)
{
}

QString QxrdFitter::reasonString() const
{
  QString res = "Unknown";

  switch (m_Reason) {
  case NoResult:
    res = "No Result";
    break;

  case OutsideData:
    res = "Outside range of data";
    break;

  case Successful:
    res = "Successful";
    break;

  case BadWidth:
    res = "Bad Width";
    break;

  case BadPosition:
    res = "Bad Position";
    break;

  case BadHeight:
    res = "Bad Height";
    break;
  }

  return res;
}
