#include "qceproipolygon.h"

QcepROIPolygon::QcepROIPolygon(QString name, QPolygonF poly)
  : inherited(name, PolygonShape),
    m_Poly(this, "poly", poly, "ROI Polygon Shape")
{
}

QcepROIPolygon::QcepROIPolygon(QString name, double scale)
  : inherited(name, PolygonShape),
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

void QcepROIPolygon::clear()
{
  m_Poly.clear();

//  set_Coords(QRectF());

  emit roiChanged();
}

void QcepROIPolygon::append(double x, double y)
{
  append(QPointF(x,y));
}

void QcepROIPolygon::append(QPointF pt)
{
  QcepPolygon p = get_Poly();
  p.append(pt);

  QPolygonF pf(p);

//  set_Coords(pf.boundingRect());
  set_Poly(p);

  emit roiChanged();
}

void QcepROIPolygon::setPoint(int n, QPointF val)
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

QPointF QcepROIPolygon::point(int n)
{
  return get_Poly().value(n);
}

double QcepROIPolygon::x(int n)
{
  return point(n).x();
}

double QcepROIPolygon::y(int n)
{
  return point(n).y();
}

int QcepROIPolygon::count()
{
  return get_Poly().size();
}

int QcepROIPolygon::markerCount()
{
  return get_Poly().count() + 1;
}

QPointF QcepROIPolygon::markerPoint(int n)
{
  QcepPolygon p = get_Poly();

  if (n >= 0 && n < p.count()) {
    return p.value(n);
  } else {
    return p.value(0);
  }
}

QPolygonF QcepROIPolygon::dragOutline()
{
  return get_Poly();
}

QRectF QcepROIPolygon::boundingRect()
{
  QcepPolygon qp = get_Poly();

  QPolygonF  p(qp);

  return p.boundingRect();
}

bool QcepROIPolygon::pointInShape(QPointF pt)
{
  QcepPolygon qp = get_Poly();

  QPolygonF p(qp);

  bool isIn = p.containsPoint(pt, Qt::WindingFill);

  return isIn;
}

void QcepROIPolygon::scale(double kx, double ky)
{
  QcepPolygon qp = get_Poly();

  QcepPolygon res = QcepPolygon();

  for (int i=0; i<qp.count(); i++) {
    QPointF pt = qp.value(i);

    res.append(QPointF(pt.x()*kx, pt.y()*ky));
  }

  set_Poly(res);
}

void QcepROIPolygon::deleteROIPoint(int n)
{
  QcepPolygon qp = get_Poly();

  qp.remove(n);

  qp.last() = qp.first();

  set_Poly(qp);
}

void QcepROIPolygon::changeROIPoint(int n, QPointF pt)
{
  QcepPolygon qp = get_Poly();

  qp[n] = pt;

  qp.last() = qp.first();

  set_Poly(qp);
}

void QcepROIPolygon::insertROIPoint(int n, QPointF pt)
{
  QcepPolygon qp = get_Poly();

  qp.insert(n+1, pt);

  qp.last() = qp.first();

  set_Poly(qp);
}
