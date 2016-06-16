#include "qxrdroiellipse.h"

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

bool QxrdROIEllipse::pointInShape(QPointF pt)
{
  printf("Need QxrdROIEllipse::pointInShape\n");

  return boundingRect().contains(pt);
}
