#include "qxrdroipolygon.h"

QxrdROIPolygon::QxrdROIPolygon(QPolygonF poly)
  : QxrdROIShape(PolygonShape, poly.boundingRect()),
    m_Poly(this, "poly", poly, "ROI Polygon Shape")
{
}

QxrdROIPolygon::QxrdROIPolygon()
  : QxrdROIShape(PolygonShape, QRectF()),
    m_Poly(this, "poly", QPolygonF(), "ROI Polygon Shape")
{
}

void QxrdROIPolygon::clear()
{
  m_Poly.clear();

  set_Coords(QRectF());

  emit roiChanged();
}

void QxrdROIPolygon::append(double x, double y)
{
  append(QPointF(x,y));
}

void QxrdROIPolygon::append(QPointF pt)
{
  QcepPolygon p = get_Poly();
  p.append(pt);

  QPolygonF pf(p);

  set_Coords(pf.boundingRect());
  set_Poly(p);

  emit roiChanged();
}

void QxrdROIPolygon::setPoint(int n, QPointF val)
{
  QcepPolygon p = get_Poly();

  if (n >= 0 && n < p.count()) {
    p[n] = val;
  }

  QPolygonF pf(p);

  set_Coords(pf.boundingRect());
  set_Poly(p);

  emit roiChanged();
}

QPointF QxrdROIPolygon::point(int n)
{
  return get_Poly().value(n);
}

double QxrdROIPolygon::x(int n)
{
  return point(n).x();
}

double QxrdROIPolygon::y(int n)
{
  return point(n).y();
}

int QxrdROIPolygon::count()
{
  return get_Poly().size();
}
