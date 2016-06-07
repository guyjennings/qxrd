#include "qxrdroirectangle.h"

QxrdROIRectangle::QxrdROIRectangle(double left,
                                   double top,
                                   double right,
                                   double bottom)
  : QxrdROIShape(RectangleShape, left, top, right, bottom)
{

}

QxrdROIRectangle::QxrdROIRectangle()
  : QxrdROIShape(RectangleShape, 0,0, 50, 50)
{
}

int QxrdROIRectangle::markerCount()
{
  return 5;
}

QPointF QxrdROIRectangle::markerPoint(int n)
{
  QPointF res(0,0);

  switch (n) {
  case 0:
  case 4:
    res = get_Coords().topLeft();
    break;

  case 1:
    res = get_Coords().topRight();
    break;

  case 2:
    res = get_Coords().bottomRight();
    break;

  case 3:
    res = get_Coords().bottomLeft();
  }

  return res;
}
