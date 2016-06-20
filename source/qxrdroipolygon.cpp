#include "qxrdroipolygon.h"

QxrdROIPolygon::QxrdROIPolygon(QPolygonF poly)
  : QxrdROIShape(PolygonShape),
    m_Poly(this, "poly", poly, "ROI Polygon Shape")
{
}

QxrdROIPolygon::QxrdROIPolygon(double scale)
  : QxrdROIShape(PolygonShape),
    m_Poly(this, "poly", QPolygonF(), "ROI Polygon Shape")
{
  m_Poly.appendValue(QPointF(-50,0)*scale);
  m_Poly.appendValue(QPointF(-40,50)*scale);
  m_Poly.appendValue(QPointF( 40,50)*scale);
  m_Poly.appendValue(QPointF( 50,0)*scale);
  m_Poly.appendValue(QPointF( 40,-50)*scale);
  m_Poly.appendValue(QPointF(-40,-50)*scale);
  m_Poly.appendValue(QPointF(-50,0)*scale);
}

void QxrdROIPolygon::clear()
{
  m_Poly.clear();

//  set_Coords(QRectF());

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

//  set_Coords(pf.boundingRect());
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

//  set_Coords(pf.boundingRect());
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

int QxrdROIPolygon::markerCount()
{
  return get_Poly().count() + 1;
}

QPointF QxrdROIPolygon::markerPoint(int n)
{
  QcepPolygon p = get_Poly();

  if (n >= 0 && n < p.count()) {
    return p.value(n);
  } else {
    return p.value(0);
  }
}

QPolygonF QxrdROIPolygon::dragOutline()
{
  return get_Poly();
}

QRectF QxrdROIPolygon::boundingRect()
{
  QcepPolygon qp = get_Poly();

  QPolygonF  p(qp);

  return p.boundingRect();
}

bool QxrdROIPolygon::pointInShape(QPointF pt)
{
  QcepPolygon qp = get_Poly();

  QPolygonF p(qp);

  bool isIn = p.containsPoint(pt, Qt::WindingFill);

  return isIn;
}

void QxrdROIPolygon::scale(double kx, double ky)
{
  QcepPolygon qp = get_Poly();

  QcepPolygon res = QcepPolygon();

  for (int i=0; i<qp.count(); i++) {
    QPointF pt = qp.value(i);

    res.append(QPointF(pt.x()*kx, pt.y()*ky));
  }

  set_Poly(res);
}

void QxrdROIPolygon::deleteROIPoint(int n)
{
  QcepPolygon qp = get_Poly();

  qp.remove(n);

  set_Poly(qp);
}

void QxrdROIPolygon::changeROIPoint(int n, QPointF pt)
{
  QcepPolygon qp = get_Poly();

  qp[n] = pt;

  set_Poly(qp);
}

void QxrdROIPolygon::insertROIPoint(int n, QPointF pt)
{
  QcepPolygon qp = get_Poly();

  qp.insert(n+1, pt);

  set_Poly(qp);
}