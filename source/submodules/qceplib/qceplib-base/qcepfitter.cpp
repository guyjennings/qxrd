#include "qcepfitter.h"

QcepFitter::QcepFitter(QcepCenterFinder *cf) :
  m_CenterFinder(cf),
  m_Reason(NoResult)
{
}

QcepFitter::QcepFitter() :
  m_CenterFinder(NULL),
  m_Reason(NoResult)
{
}

QString QcepFitter::reasonString() const
{
  return QcepFitter::reasonString(m_Reason);
}

QString QcepFitter::reasonString(FitResult reason)
{
  QString res = "Unknown";

  switch (reason) {

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

  default:
    break;
  }

  return res;
}

QString QcepFitter::reasonString(double v)
{
  return reasonString((FitResult) (int) v);
}
