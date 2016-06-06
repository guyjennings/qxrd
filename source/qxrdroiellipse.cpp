#include "qxrdroiellipse.h"

QxrdROIEllipse::QxrdROIEllipse(double left, double top, double right, double bottom)
  : QxrdROIShape(EllipseShape, left, top, right, bottom)
{
}

QxrdROIEllipse::QxrdROIEllipse()
  : QxrdROIShape(EllipseShape, 0, 0, 50, 50)
{
}
