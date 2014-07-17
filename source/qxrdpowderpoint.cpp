#include "qxrdpowderpoint.h"

QxrdPowderPoint::QxrdPowderPoint(int n1, int n2, double x, double y) :
  m_N1(n1),
  m_N2(n2),
  m_X(x),
  m_Y(y)
{
}

QxrdPowderPoint::QxrdPowderPoint() :
  m_N1(0),
  m_N2(0),
  m_X(0.0),
  m_Y(0.0)
{
}

bool QxrdPowderPoint::operator == (const QxrdPowderPoint &pt) const
{
  return n1()==pt.n1() && n2()==pt.n2() && x()==pt.x() && y()==pt.y();
}

bool QxrdPowderPoint::operator != (const QxrdPowderPoint &pt) const
{
  return n1()!=pt.n1() || n2()!=pt.n2() || x()!=pt.x() || y()!=pt.y();
}
