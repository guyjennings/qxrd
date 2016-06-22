#include "qxrdroiellipse.h"
#include <cmath>

static const int nPts = 16;

QxrdROIEllipse::QxrdROIEllipse(double halfWidth, double halfHeight)
  : QxrdROICenteredShape(EllipseShape, halfWidth, halfHeight)
{
}

QxrdROIEllipse::QxrdROIEllipse(double scale)
  : QxrdROICenteredShape(EllipseShape, 50*scale, 50*scale)
{
}

int QxrdROIEllipse::markerCount()
{
  return nPts+1;
}

QPointF QxrdROIEllipse::markerPoint(int n)
{
  double a=get_HalfWidth();
  double b=get_HalfHeight();

  double theta = M_PI*n/(nPts/2.0);
  double x = a*cos(theta);
  double y = b*sin(theta);

  return QPointF(x,y);
}

QPolygonF QxrdROIEllipse::dragOutline()
{
  QPolygonF res;

  for (int i=0; i<=nPts; i++) {
    res.append(markerPoint(i));
  }

  return res;
}

bool QxrdROIEllipse::pointInShape(QPointF pt)
{
  double a=get_HalfWidth();
  double b=get_HalfHeight();

  double x=pt.x()/a;
  double y=pt.y()/b;

  double r= sqrt(x*x+y*y);

  return (r <= 1.0);
}
