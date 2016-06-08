#include "qxrdroiellipse.h"

static const int nPts = 64;

QxrdROIEllipse::QxrdROIEllipse(double left, double top, double right, double bottom)
  : QxrdROIShape(EllipseShape, left, top, right, bottom)
{
}

QxrdROIEllipse::QxrdROIEllipse(double scale)
  : QxrdROIShape(EllipseShape, -50*scale, -50*scale, 50*scale, 50*scale)
{
}

int QxrdROIEllipse::markerCount()
{
  return nPts+1;
}

QPointF QxrdROIEllipse::markerPoint(int n)
{
  QRectF c=get_Coords();

  double a=c.width()/2.0;
  double b=c.height()/2.0;
  QPointF cen=c.center();

  double theta = M_PI*n/(nPts/2.0);
  double x = cen.x() + a*cos(theta);
  double y = cen.y() + b*sin(theta);

  return QPointF(x,y);
}

QRectF QxrdROIEllipse::boundingRect()
{
  return get_Coords();
}

bool QxrdROIEllipse::pointInShape(QPointF pt)
{
  printf("Need QxrdROIEllipse::pointInShape\n");

  return get_Coords().contains(pt);
}
