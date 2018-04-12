#include "qceproiellipse.h"
#include "qmath.h"

static const int nPts = 16;

QcepROIEllipse::QcepROIEllipse(QString name, double halfWidth, double halfHeight)
  : QcepROICenteredShape(name, EllipseShape, halfWidth, halfHeight)
{
}

QcepROIEllipse::QcepROIEllipse(QString name, double scale)
  : QcepROICenteredShape(name, EllipseShape, 50*scale, 50*scale)
{
}

int QcepROIEllipse::markerCount()
{
  return nPts+1;
}

QPointF QcepROIEllipse::markerPoint(int n)
{
  double a=get_HalfWidth();
  double b=get_HalfHeight();

  double theta = M_PI*n/(nPts/2.0);
  double x = a*cos(theta);
  double y = b*sin(theta);

  return QPointF(x,y);
}

QPolygonF QcepROIEllipse::dragOutline()
{
  QPolygonF res;

  for (int i=0; i<=nPts; i++) {
    res.append(markerPoint(i));
  }

  return res;
}

bool QcepROIEllipse::pointInShape(QPointF pt)
{
  double a=get_HalfWidth();
  double b=get_HalfHeight();

  double x=pt.x()/a;
  double y=pt.y()/b;

  double r= sqrt(x*x+y*y);

  return (r <= 1.0);
}
