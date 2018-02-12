#include "qxrdroirectangle.h"

QxrdROIRectangle::QxrdROIRectangle(QString name,
                                   double halfWidth,
                                   double halfHeight)
  : inherited(name, RectangleShape, halfWidth, halfHeight)
{

}

QxrdROIRectangle::QxrdROIRectangle(QString name, double scale)
  : inherited(name, RectangleShape, scale*50, scale*50)
{
}

int QxrdROIRectangle::markerCount()
{
  return 5;
}

QPointF QxrdROIRectangle::markerPoint(int n)
{
  QPointF res(0,0);
  double hw = get_HalfWidth();
  double hh = get_HalfHeight();

  switch (n) {
  case 0:
  case 4:
    res = QPointF(-hw,hh);
    break;

  case 1:
    res = QPointF(hw,hh);
    break;

  case 2:
    res = QPointF(hw,-hh);
    break;

  case 3:
    res = QPointF(-hw,-hh);
  }

  return res;
}

QPolygonF QxrdROIRectangle::dragOutline()
{
  QPolygonF res(boundingRect());

  return res;
}

bool QxrdROIRectangle::pointInShape(QPointF pt)
{
  return boundingRect().contains(pt);
}
