#include "qxrdfitter.h"

QxrdFitter::QxrdFitter(QxrdCenterFinder *cf) :
  m_CenterFinder(cf),
  m_Reason(NoResult)
{
}

QxrdFitter::QxrdFitter() :
  m_CenterFinder(NULL),
  m_Reason(NoResult)
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
