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
